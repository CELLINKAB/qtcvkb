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

#ifndef VKBQUICKMODEL_H
#define VKBQUICKMODEL_H

#include <QtCore/qobject.h>
#include <QtQml/qqmllist.h>

class VkbInputKey;
class VkbQuickPopup;
class VkbQuickDelegate;

QT_FORWARD_DECLARE_CLASS(QQuickItem)
QT_FORWARD_DECLARE_CLASS(QQuickAbstractButton)

class VkbQuickModel : public QObject
{
    Q_OBJECT

public:
    explicit VkbQuickModel(QObject *parent = nullptr);

    QQmlListProperty<VkbQuickDelegate> delegates();

    VkbQuickDelegate *findDelegate(Qt::Key key) const;
    QQuickAbstractButton *createButton(const VkbInputKey &key, QQuickItem *parent) const;
    VkbQuickPopup *createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const;

signals:
    void delegatesChanged();

private:
    static void delegates_append(QQmlListProperty<VkbQuickDelegate> *property, VkbQuickDelegate *delegate);
    static int delegates_count(QQmlListProperty<VkbQuickDelegate> *property);
    static VkbQuickDelegate *delegates_at(QQmlListProperty<VkbQuickDelegate> *property, int index);
    static void delegates_clear(QQmlListProperty<VkbQuickDelegate> *property);

    QList<VkbQuickDelegate *> m_delegates;
};

#endif // VKBQUICKMODEL_H
