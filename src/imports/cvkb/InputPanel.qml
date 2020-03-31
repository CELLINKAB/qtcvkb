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

import QtQuick 2.14
import QtQuick.Controls 2.14
import QtCellink.Vkb 0.1
import QtCellink.Vkb.Templates 0.1 as T

T.InputPanel {
    id: control

    y: parent.height
    width: parent.width
    height: parent.height / 2
    spacing: 10
    padding: 10

    enter: Transition {
        NumberAnimation { property: "y"; from: parent.height; to: parent.height - control.height; easing.type: Easing.InOutCubic }
    }

    exit: Transition {
        NumberAnimation { property: "y"; from: parent.height - control.height; to: parent.height; easing.type: Easing.InOutCubic }
    }

    contentItem: T.InputLayoutItem {
        spacing: control.spacing
        delegates: [
            T.InputLayoutDelegate { button: InputKey { } popup: InputPopup { } },
            T.InputLayoutDelegate { key: Qt.Key_Meta; button: InputKey { text: "123#" } popup: InputPopup { } },
            T.InputLayoutDelegate { key: Qt.Key_Enter; button: InputKey { text: "\u23ce" } popup: InputPopup { } },
            T.InputLayoutDelegate { key: Qt.Key_Shift; button: InputKey { text: "\u21e7" } popup: InputPopup { } },
            T.InputLayoutDelegate { key: Qt.Key_Escape; button: InputKey { text: "\u21e9" } popup: InputPopup { } },
            T.InputLayoutDelegate { key: Qt.Key_Backspace; button: InputKey { text: "\u21e6" } popup: InputPopup { } }
        ]
    }

    background: Rectangle {
        color: control.palette.window
        Rectangle {
            width: parent.width
            height: 1
            color: control.palette.mid
        }
    }
}
