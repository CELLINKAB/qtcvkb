/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2020 CELLINK AB <info@cellink.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vkbinputlayout.h"

#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>

#include <cmath>

static const QString Key = QStringLiteral("key");
static const QString Alt = QStringLiteral("alt");
static const QString Span = QStringLiteral("span");
static const QString AutoRepeat = QStringLiteral("autoRepeat");
static const QString Checkable = QStringLiteral("checkable");
static const QString Checked = QStringLiteral("checked");

class VkbInputLayoutPrivate : public QSharedData
{
public:
    int maxColumns = 0;
    QVector<QVector<VkbInputKey>> layout;
};

VkbInputLayout::VkbInputLayout() : d_ptr(new VkbInputLayoutPrivate)
{
}

VkbInputLayout::VkbInputLayout(const VkbInputLayout &other) : d_ptr(other.d_ptr)
{
}

VkbInputLayout::~VkbInputLayout()
{
}

VkbInputLayout &VkbInputLayout::operator=(const VkbInputLayout &other)
{
    d_ptr = other.d_ptr;
    return *this;
}

bool VkbInputLayout::operator==(const VkbInputLayout &other) const
{
    return d_ptr == other.d_ptr;
}

bool VkbInputLayout::operator!=(const VkbInputLayout &other) const
{
    return !(*this == other);
}

int VkbInputLayout::rowCount() const
{
    return d_ptr->layout.count();
}

int VkbInputLayout::columnCount() const
{
    return d_ptr->maxColumns;
}

QVector<VkbInputKey> VkbInputLayout::rowAt(int row) const
{
    return d_ptr->layout.value(row);
}

VkbInputKey VkbInputLayout::keyAt(int row, int column) const
{
    return d_ptr->layout.value(row).value(column);
}

// ### TODO: add QJsonArray::toStringList()
static QStringList toStringList(const QJsonArray &array)
{
    QStringList strings;
    strings.reserve(array.size());
    for (const QJsonValue &value : array)
        strings += value.toString();
    return strings;
}

static VkbInputKey parseKey(const QJsonObject &json)
{
    VkbInputKey key;
    key.key = json.value(Key).toString();
    key.alt = toStringList(json.value(Alt).toArray());
    key.span = json.value(Span).toDouble(1);
    key.autoRepeat = json.value(AutoRepeat).toBool();
    key.checkable = json.value(Checkable).toBool();
    key.checked = json.value(Checked).toBool();
    return key;
}

static QVector<VkbInputKey> parseRow(const QJsonArray &json, int &maxColumns)
{
    QVector<VkbInputKey> keys;
    for (const QJsonValue &value : json)
        keys += parseKey(value.toObject());
    maxColumns = std::max(keys.count(), maxColumns);
    return keys;
}

static QVector<QVector<VkbInputKey>> parseLayout(const QJsonArray &json, int &maxColumns)
{
    QVector<QVector<VkbInputKey>> layout;
    for (const QJsonValue &value : json)
        layout += parseRow(value.toArray(), maxColumns);
    return layout;
}

bool VkbInputLayout::load(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning().nospace().noquote() << "VkbInputLayout::load: error opening " << filePath << ": " << file.errorString();
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning().nospace().noquote() << "VkbInputLayout::load: error parsing " << filePath << ": " << error.errorString();
        return false;
    }

    d_ptr->maxColumns = 0;
    d_ptr->layout = parseLayout(doc.array(), d_ptr->maxColumns);
    return true;
}
