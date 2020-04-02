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
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtCellink.Vkb 0.1

ApplicationWindow {
    id: window
    width: 960
    height: 540
    visible: true

    SplitView {
        anchors.fill: parent

        Pane {
            padding: 0
            height: parent.height
            focusPolicy: Qt.ClickFocus

            SplitView.fillWidth: true

            TextField {
                id: textField
                width: parent.width / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -height
            }
        }

        Pane {
            padding: 0
            height: parent.height
            palette.window: window.palette.base

            SplitView.minimumWidth: window.width / 6
            SplitView.preferredWidth: window.width / 3
            SplitView.maximumWidth: window.width / 2

            ListView {
                anchors.fill: parent
                anchors.bottomMargin: Qt.inputMethod.visible ? window.height - Qt.inputMethod.keyboardRectangle.y : 0

                header: ColumnLayout {
                    spacing: 0
                    width: parent.width

                    Label {
                        text: "Echo:"
                        font.bold: true
                        padding: textField.padding
                        background: Rectangle {
                            color: palette.window
                        }
                        Layout.fillWidth: true
                    }

                    ComboBox {
                        currentIndex: textField.echoMode
                        model: [
                            "Normal",
                            "Password",
                            "NoEcho",
                            "PasswordEchoOnEdit"
                        ]
                        Layout.fillWidth: true
                        onActivated: textField.echoMode = currentIndex
                    }
                }

                model: ListModel {
                    ListElement { hint: "ImhDigitsOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhFormattedNumbersOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhUppercaseOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhLowercaseOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhDialableCharactersOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhEmailCharactersOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhUrlCharactersOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhLatinOnly"; section: "Restrict:" }
                    ListElement { hint: "ImhHiddenText"; section: "Behavior:" }
                    ListElement { hint: "ImhSensitiveData"; section: "Behavior:" }
                    ListElement { hint: "ImhNoAutoUppercase"; section: "Behavior:" }
                    ListElement { hint: "ImhPreferNumbers"; section: "Behavior:" }
                    ListElement { hint: "ImhPreferUppercase"; section: "Behavior:" }
                    ListElement { hint: "ImhPreferLowercase"; section: "Behavior:" }
                    ListElement { hint: "ImhNoPredictiveText"; section: "Behavior:" }
                    ListElement { hint: "ImhDate"; section: "Behavior:" }
                    ListElement { hint: "ImhTime"; section: "Behavior:" }
                    ListElement { hint: "ImhPreferLatin"; section: "Behavior:" }
                    ListElement { hint: "ImhMultiLine"; section: "Behavior:" }
                    ListElement { hint: "ImhNoEditMenu"; section: "Behavior:" }
                    ListElement { hint: "ImhNoTextHandles"; section: "Behavior:" }
                }

                delegate: CheckDelegate {
                    text: model.hint
                    width: parent.width
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    checked: textField.inputMethodHints & Qt[model.hint]
                    onToggled: textField.inputMethodHints ^= Qt[model.hint]
                }

                section.property: "section"
                section.delegate: Label {
                    text: section
                    font.bold: true
                    width: parent.width
                    padding: textField.padding
                    background: Rectangle {
                        color: window.palette.window
                    }
                }

                ScrollBar.vertical: ScrollBar { id: scrollBar }
            }
        }
    }
}
