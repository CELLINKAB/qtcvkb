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

#include <QtTest/qtest.h>
#include <QtCVkb/vkbinputlayout.h>

class tst_vkbinputlayout : public QObject
{
    Q_OBJECT

private slots:
    void load_data();
    void load();
    void fail_data();
    void fail();
    void rows_data();
    void rows();
    void keys_data();
    void keys();
};

void tst_vkbinputlayout::load_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<int>("expectedRows");
    QTest::addColumn<int>("expectedColumns");

    QTest::newRow("letters.json") << QFINDTESTDATA("data/letters.json") << 4 << 11;
    QTest::newRow("digits.json") << QFINDTESTDATA("data/digits.json") << 4 << 4;
}

void tst_vkbinputlayout::load()
{
    QFETCH(QString, filePath);
    QFETCH(int, expectedRows);
    QFETCH(int, expectedColumns);

    VkbInputLayout layout;
    QVERIFY(layout.load(filePath));
    QCOMPARE(layout.rowCount(), expectedRows);
    QCOMPARE(layout.columnCount(), expectedColumns);
}

void tst_vkbinputlayout::fail_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("expectedError");

    QTest::newRow("non-existent") << "non-existent.json" << "VkbInputLayout::load: error opening non-existent.json: No such file or directory";
    QTest::newRow("empty") << QFINDTESTDATA("data/empty.json") << QStringLiteral("VkbInputLayout::load: error parsing %1: illegal value").arg(QFINDTESTDATA("data/empty.json"));
}

void tst_vkbinputlayout::fail()
{
    QFETCH(QString, filePath);
    QFETCH(QString, expectedError);

    VkbInputLayout layout;
    QTest::ignoreMessage(QtWarningMsg, qPrintable(expectedError));
    QVERIFY(!layout.load(filePath));
    QCOMPARE(layout.rowCount(), 0);
    QCOMPARE(layout.columnCount(), 0);
}

void tst_vkbinputlayout::rows_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("expectedCount");

    QTest::newRow("l0") << "letters.json" << 0 << 11;
    QTest::newRow("l1") << "letters.json" << 1 << 10;
    QTest::newRow("l2") << "letters.json" << 2 << 10;
    QTest::newRow("l3") << "letters.json" << 3 << 5;

    QTest::newRow("d0") << "digits.json" << 0 << 4;
    QTest::newRow("d1") << "digits.json" << 1 << 4;
    QTest::newRow("d2") << "digits.json" << 2 << 4;
    QTest::newRow("d3") << "digits.json" << 3 << 4;
}

void tst_vkbinputlayout::rows()
{
    QFETCH(QString, fileName);
    QFETCH(int, row);
    QFETCH(int, expectedCount);

    VkbInputLayout layout;
    QVERIFY(layout.load(QFINDTESTDATA("data/" + fileName)));

    QCOMPARE(layout.rowAt(row).count(), expectedCount);
}

void tst_vkbinputlayout::keys_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<int>("row");
    QTest::addColumn<int>("column");
    QTest::addColumn<Qt::Key>("expectedKey");
    QTest::addColumn<QString>("expectedText");
    QTest::addColumn<QStringList>("expectedAlt");
    QTest::addColumn<qreal>("expectedSpan");
    QTest::addColumn<bool>("expectedAutoRepeat");
    QTest::addColumn<bool>("expectedCheckable");
    QTest::addColumn<bool>("expectedChecked");

    QTest::newRow("q") << "letters.json" << 0 << 0 << Qt::Key_unknown << "q" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("w") << "letters.json" << 0 << 1 << Qt::Key_unknown << "w" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("e") << "letters.json" << 0 << 2 << Qt::Key_unknown << "e" << QStringList({"è", "é", "ê", "ë", "ē"}) << 1.0 << false << false << false;
    QTest::newRow("r") << "letters.json" << 0 << 3 << Qt::Key_unknown << "r" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("t") << "letters.json" << 0 << 4 << Qt::Key_unknown << "t" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("y") << "letters.json" << 0 << 5 << Qt::Key_unknown << "y" << QStringList() << 1.0 << false << false << false;

    QTest::newRow("a") << "letters.json" << 1 << 0 << Qt::Key_unknown << "a" << QStringList({"à", "á", "â", "ä", "æ", "ã", "å", "ā"}) << 1.0 << false << false << false;
    QTest::newRow("s") << "letters.json" << 1 << 1 << Qt::Key_unknown << "s" << QStringList({"ß"}) << 1.0 << false << false << false;
    QTest::newRow("d") << "letters.json" << 1 << 2 << Qt::Key_unknown << "d" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("f") << "letters.json" << 1 << 3 << Qt::Key_unknown << "f" << QStringList() << 1.0 << false << false << false;

    QTest::newRow("backspace") << "letters.json" << 0 << 10 << Qt::Key_Backspace << "" << QStringList() << 1.0 << true << false << false;
    QTest::newRow("enter") << "letters.json" << 1 << 9 << Qt::Key_Enter << "" << QStringList() << 2.0 << false << false << false;
    QTest::newRow("shift") << "letters.json" << 2 << 0 << Qt::Key_Shift << "" << QStringList() << 2.0 << false << true << false;
    QTest::newRow("meta") << "letters.json" << 3 << 0 << Qt::Key_Meta << "" << QStringList() << 1.0 << false << true << false;
    QTest::newRow("space") << "letters.json" << 3 << 2 << Qt::Key_Space << "" << QStringList() << 7.0 << true << false << false;
    QTest::newRow("escape") << "letters.json" << 3 << 4 << Qt::Key_Escape << "" << QStringList() << 1.0 << false << false << false;

    QTest::newRow("1") << "digits.json" << 0 << 0 << Qt::Key_unknown << "1" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("2") << "digits.json" << 0 << 1 << Qt::Key_unknown << "2" << QStringList() << 1.0 << false << false << false;
    QTest::newRow("3") << "digits.json" << 0 << 2 << Qt::Key_unknown << "3" << QStringList() << 1.0 << false << false << false;

    QTest::newRow(",") << "digits.json" << 3 << 0 << Qt::Key_unknown << "," << QStringList() << 1.0 << false << false << false;
    QTest::newRow("0") << "digits.json" << 3 << 1 << Qt::Key_unknown << "0" << QStringList() << 1.0 << false << false << false;
    QTest::newRow(".") << "digits.json" << 3 << 2 << Qt::Key_unknown << "." << QStringList() << 1.0 << false << false << false;
}

void tst_vkbinputlayout::keys()
{
    QFETCH(QString, fileName);
    QFETCH(int, row);
    QFETCH(int, column);
    QFETCH(Qt::Key, expectedKey);
    QFETCH(QString, expectedText);
    QFETCH(QStringList, expectedAlt);
    QFETCH(qreal, expectedSpan);
    QFETCH(bool, expectedAutoRepeat);
    QFETCH(bool, expectedCheckable);
    QFETCH(bool, expectedChecked);

    VkbInputLayout layout;
    QVERIFY(layout.load(QFINDTESTDATA("data/" + fileName)));

    VkbInputKey key = layout.keyAt(row, column);
    QCOMPARE(key.key, expectedKey);
    QCOMPARE(key.text, expectedText);
    QCOMPARE(key.alt, expectedAlt);
    QCOMPARE(key.span, expectedSpan);
    QCOMPARE(key.autoRepeat, expectedAutoRepeat);
    QCOMPARE(key.checkable, expectedCheckable);
    QCOMPARE(key.checked, expectedChecked);
}

QTEST_MAIN(tst_vkbinputlayout)

#include "tst_vkbinputlayout.moc"
