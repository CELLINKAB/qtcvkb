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

#include "vkbquickeditor.h"

#include <QtQuick/private/qquickevents_p_p.h>
#include <QtQuick/private/qquicktextinput_p_p.h>
#include <QtQuickTemplates2/private/qquicktextfield_p.h>

VkbQuickEditor::VkbQuickEditor(QObject *parent) : QObject(parent)
{
    if (QQuickTextField *textField = qobject_cast<QQuickTextField *>(parent)) {
        connect(textField, &QQuickTextField::pressed, this, &VkbQuickEditor::handlePress);
        connect(textField, &QQuickTextField::released, this, &VkbQuickEditor::handleRelease);
        connect(textField, &QQuickTextField::pressAndHold, this, &VkbQuickEditor::handlePressAndHold);
    }
}

int VkbQuickEditor::cursorPositionAt(const QPointF &pos) const
{
    QQuickTextField *textField = qobject_cast<QQuickTextField *>(parent());
    if (!textField)
        return -1;

    QQuickTextInputPrivate *d = QQuickTextInputPrivate::get(textField);
    return d->positionAt(pos);
}

void VkbQuickEditor::setCursorPosition(int cursorPosition)
{
    QQuickTextField *textField = qobject_cast<QQuickTextField *>(parent());
    if (!textField)
        return;

    textField->setCursorPosition(cursorPosition);
}

void VkbQuickEditor::selectWord()
{
    QQuickTextField *textField = qobject_cast<QQuickTextField *>(parent());
    if (!textField)
        return;

    textField->selectWord();
}

void VkbQuickEditor::handlePress(QQuickMouseEvent *event)
{
    emit pressed(QPointF(event->x(), event->y()));
}

void VkbQuickEditor::handleRelease(QQuickMouseEvent *event)
{
    emit released(QPointF(event->x(), event->y()));
}

void VkbQuickEditor::handlePressAndHold(QQuickMouseEvent *event)
{
    emit pressAndHold(QPointF(event->x(), event->y()));
}
