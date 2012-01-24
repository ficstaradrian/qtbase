/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest>
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qjsondocument.h"

class TestQtJson: public QObject
{
    Q_OBJECT
public:
    TestQtJson(QObject *parent = 0);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testValueSimple();
    void testNumbers();

    void testObjectSimple();
    void testObjectSmallKeys();
    void testArraySimple();
    void testValueObject();
    void testValueArray();
    void testObjectNested();
    void testArrayNested();
    void testArrayNestedEmpty();
    void testObjectNestedEmpty();

    void testValueRef();
    void testObjectIteration();
    void testArrayIteration();

    void testObjectFind();

    void testDocument();

    void nullValues();
    void nullArrays();
    void nullObject();

    void keySorting();

    void undefinedValues();

    void fromVariantMap();
    void toVariantMap();

    void toJson();
    void fromJson();
    void fromBinary();
    void toAndFromBinary_data();
    void toAndFromBinary();
    void parseNumbers();
    void parseStrings();
    void parseDuplicateKeys();
    void testParser();

    void compactArray();
    void compactObject();

    void validation();

    void assignToDocument();

    void testDuplicateKeys();
    void testCompaction();
    void testDebugStream();
    void testCompactionError();
};

TestQtJson::TestQtJson(QObject *parent) : QObject(parent)
{
}

void TestQtJson::initTestCase()
{
}

void TestQtJson::cleanupTestCase()
{
}

void TestQtJson::init()
{
}

void TestQtJson::cleanup()
{
}

void TestQtJson::testValueSimple()
{
    QJsonValue value(true);
    QCOMPARE(value.type(), QJsonValue::Bool);
    QCOMPARE(value.toDouble(), 0.);
    QCOMPARE(value.toString(), QString());
    QCOMPARE(value.toBool(), true);
    QCOMPARE(value.toObject(), QJsonObject());
    QCOMPARE(value.toArray(), QJsonArray());

    value = 999.;
    QCOMPARE(value.type(), QJsonValue::Double);
    QCOMPARE(value.toDouble(), 999.);
    QCOMPARE(value.toString(), QString());
    QCOMPARE(value.toBool(), false);
    QCOMPARE(value.toObject(), QJsonObject());
    QCOMPARE(value.toArray(), QJsonArray());

    value = QLatin1String("test");
    QCOMPARE(value.toDouble(), 0.);
    QCOMPARE(value.toString(), QLatin1String("test"));
    QCOMPARE(value.toBool(), false);
    QCOMPARE(value.toObject(), QJsonObject());
    QCOMPARE(value.toArray(), QJsonArray());

    value = true;
    QCOMPARE(value.toDouble(), 0.);
    QCOMPARE(value.toString(), QString());
    QCOMPARE(value.toBool(), true);
    QCOMPARE(value.toObject(), QJsonObject());
    QCOMPARE(value.toArray(), QJsonArray());

    value = 999.;
    QCOMPARE(value.toDouble(), 999.);
    QCOMPARE(value.toString(), QString());
    QCOMPARE(value.toBool(), false);
    QCOMPARE(value.toObject(), QJsonObject());
    QCOMPARE(value.toArray(), QJsonArray());

}

void TestQtJson::testNumbers()
{
    {
        int numbers[] = {
            0,
            -1,
            1,
            (1<<26),
            (1<<27),
            (1<<28),
            -(1<<26),
            -(1<<27),
            -(1<<28),
            (1<<26) - 1,
            (1<<27) - 1,
            (1<<28) - 1,
            -((1<<26) - 1),
            -((1<<27) - 1),
            -((1<<28) - 1)
        };
        int n = sizeof(numbers)/sizeof(int);

        QJsonArray array;
        for (int i = 0; i < n; ++i)
            array.append((double)numbers[i]);
        for (int i = 0; i < array.size(); ++i) {
            QCOMPARE(array.at(i).type(), QJsonValue::Double);
            QCOMPARE(array.at(i).toDouble(), (double)numbers[i]);
        }
    }

    {
        double numbers[] = {
            0,
            -1,
            1,
            (1<<26),
            (1<<27),
            (1<<28),
            -(1<<26),
            -(1<<27),
            -(1<<28),
            (1<<26) - 1,
            (1<<27) - 1,
            (1<<28) - 1,
            -((1<<26) - 1),
            -((1<<27) - 1),
            -((1<<28) - 1),
            1.1,
            0.1,
            -0.1,
            -1.1,
            1e200,
            -1e200
        };
        int n = sizeof(numbers)/sizeof(double);

        QJsonArray array;
        for (int i = 0; i < n; ++i)
            array.append(numbers[i]);
        for (int i = 0; i < array.size(); ++i) {
            QCOMPARE(array.at(i).type(), QJsonValue::Double);
            QCOMPARE(array.at(i).toDouble(), numbers[i]);
        }
    }

}

void TestQtJson::testObjectSimple()
{
    QJsonObject object;
    object.insert("number", 999.);
    QCOMPARE(object.value("number").type(), QJsonValue::Double);
    QCOMPARE(object.value("number").toDouble(), 999.);
    object.insert("string", QString::fromLatin1("test"));
    QCOMPARE(object.value("string").type(), QJsonValue::String);
    QCOMPARE(object.value("string").toString(), QString("test"));
    object.insert("boolean", true);
    QCOMPARE(object.value("boolean").toBool(), true);

    QStringList keys = object.keys();
    QVERIFY2(keys.contains("number"), "key number not found");
    QVERIFY2(keys.contains("string"), "key string not found");
    QVERIFY2(keys.contains("boolean"), "key boolean not found");

    // if we put a JsonValue into the JsonObject and retreive
    // it, it should be identical.
    QJsonValue value(QLatin1String("foo"));
    object.insert("value", value);
    QCOMPARE(object.value("value"), value);

    int size = object.size();
    object.remove("boolean");
    QCOMPARE(object.size(), size - 1);
    QVERIFY2(!object.contains("boolean"), "key boolean should have been removed");

    QJsonValue taken = object.take("value");
//    QCOMPARE(taken, value);
    QVERIFY2(!object.contains("value"), "key value should have been removed");

    QString before = object.value("string").toString();
    object.insert("string", QString::fromLatin1("foo"));
    QVERIFY2(object.value("string").toString() != before, "value should have been updated");
}

void TestQtJson::testObjectSmallKeys()
{
    QJsonObject data1;
    data1.insert(QStringLiteral("1"), 123);
    QVERIFY(data1.contains(QStringLiteral("1")));
    QCOMPARE(data1.value(QStringLiteral("1")).toDouble(), (double)123);
    data1.insert(QStringLiteral("12"), 133);
    QCOMPARE(data1.value(QStringLiteral("12")).toDouble(), (double)133);
    QVERIFY(data1.contains(QStringLiteral("12")));
    data1.insert(QStringLiteral("123"), 323);
    QCOMPARE(data1.value(QStringLiteral("12")).toDouble(), (double)133);
    QVERIFY(data1.contains(QStringLiteral("123")));
    QCOMPARE(data1.value(QStringLiteral("123")).type(), QJsonValue::Double);
    QCOMPARE(data1.value(QStringLiteral("123")).toDouble(), (double)323);
}

void TestQtJson::testArraySimple()
{
    QJsonArray array;
    array.append(999.);
    array.append(QString::fromLatin1("test"));
    array.append(true);

    QJsonValue val = array.at(0);
    QCOMPARE(array.at(0).toDouble(), 999.);
    QCOMPARE(array.at(1).toString(), QString("test"));
    QCOMPARE(array.at(2).toBool(), true);
    QCOMPARE(array.size(), 3);

    // if we put a JsonValue into the JsonArray and retreive
    // it, it should be identical.
    QJsonValue value(QLatin1String("foo"));
    array.append(value);
    QCOMPARE(array.at(3), value);

    int size = array.size();
    array.removeAt(2);
    --size;
    QCOMPARE(array.size(), size);

    QJsonValue taken = array.takeAt(0);
    --size;
    QCOMPARE(taken.toDouble(), 999.);
    QCOMPARE(array.size(), size);

    // check whether null values work
    array.append(QJsonValue());
    ++size;
    QCOMPARE(array.size(), size);
    QCOMPARE(array.last().type(), QJsonValue::Null);
    QCOMPARE(array.last(), QJsonValue());

    QCOMPARE(array.first().type(), QJsonValue::String);
    QCOMPARE(array.first(), QJsonValue(QLatin1String("test")));

    array.prepend(false);
    QCOMPARE(array.first().type(), QJsonValue::Bool);
    QCOMPARE(array.first(), QJsonValue(false));

    QCOMPARE(array.at(-1), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(array.at(array.size()), QJsonValue(QJsonValue::Undefined));

    array.replace(0, -555.);
    QCOMPARE(array.first().type(), QJsonValue::Double);
    QCOMPARE(array.first(), QJsonValue(-555.));
    QCOMPARE(array.at(1).type(), QJsonValue::String);
    QCOMPARE(array.at(1), QJsonValue(QLatin1String("test")));
}

void TestQtJson::testValueObject()
{
    QJsonObject object;
    object.insert("number", 999.);
    object.insert("string", QLatin1String("test"));
    object.insert("boolean", true);

    QJsonValue value(object);

    // if we don't modify the original JsonObject, toObject()
    // on the JsonValue should return the same object (non-detached).
    QCOMPARE(value.toObject(), object);

    // if we modify the original object, it should detach
    object.insert("test", QJsonValue(QLatin1String("test")));
    QVERIFY2(value.toObject() != object, "object should have detached");
}

void TestQtJson::testValueArray()
{
    QJsonArray array;
    array.append(999.);
    array.append(QLatin1String("test"));
    array.append(true);

    QJsonValue value(array);

    // if we don't modify the original JsonArray, toArray()
    // on the JsonValue should return the same object (non-detached).
    QCOMPARE(value.toArray(), array);

    // if we modify the original array, it should detach
    array.append(QLatin1String("test"));
    QVERIFY2(value.toArray() != array, "array should have detached");
}

void TestQtJson::testObjectNested()
{
    QJsonObject inner, outer;
    inner.insert("number", 999.);
    outer.insert("nested", inner);

    // if we don't modify the original JsonObject, value()
    // should return the same object (non-detached).
    QJsonObject value = outer.value("nested").toObject();
    QCOMPARE(value, inner);
    QCOMPARE(value.value("number").toDouble(), 999.);

    // if we modify the original object, it should detach and not
    // affect the nested object
    inner.insert("number", 555.);
    value = outer.value("nested").toObject();
    QVERIFY2(inner.value("number").toDouble() != value.value("number").toDouble(),
             "object should have detached");

    // array in object
    QJsonArray array;
    array.append(123.);
    array.append(456.);
    outer.insert("array", array);
    QCOMPARE(outer.value("array").toArray(), array);
    QCOMPARE(outer.value("array").toArray().at(1).toDouble(), 456.);

    // two deep objects
    QJsonObject twoDeep;
    twoDeep.insert("boolean", true);
    inner.insert("nested", twoDeep);
    outer.insert("nested", inner);
    QCOMPARE(outer.value("nested").toObject().value("nested").toObject(), twoDeep);
    QCOMPARE(outer.value("nested").toObject().value("nested").toObject().value("boolean").toBool(),
             true);
}

void TestQtJson::testArrayNested()
{
    QJsonArray inner, outer;
    inner.append(999.);
    outer.append(inner);

    // if we don't modify the original JsonArray, value()
    // should return the same array (non-detached).
    QJsonArray value = outer.at(0).toArray();
    QCOMPARE(value, inner);
    QCOMPARE(value.at(0).toDouble(), 999.);

    // if we modify the original array, it should detach and not
    // affect the nested array
    inner.append(555.);
    value = outer.at(0).toArray();
    QVERIFY2(inner.size() != value.size(), "array should have detached");

    // objects in arrays
    QJsonObject object;
    object.insert("boolean", true);
    outer.append(object);
    QCOMPARE(outer.last().toObject(), object);
    QCOMPARE(outer.last().toObject().value("boolean").toBool(), true);

    // two deep arrays
    QJsonArray twoDeep;
    twoDeep.append(QJsonValue(QString::fromLatin1("nested")));
    inner.append(twoDeep);
    outer.append(inner);
    QCOMPARE(outer.last().toArray().last().toArray(), twoDeep);
    QCOMPARE(outer.last().toArray().last().toArray().at(0).toString(), QString("nested"));
}

void TestQtJson::testArrayNestedEmpty()
{
    QJsonObject object;
    QJsonArray inner;
    object.insert("inner", inner);
    QJsonValue val = object.value("inner");
    QJsonArray value = object.value("inner").toArray();
    QCOMPARE(value.size(), 0);
    QCOMPARE(value, inner);
    QCOMPARE(value.size(), 0);
    object.insert("count", 0.);
    QCOMPARE(object.value("inner").toArray().size(), 0);
    QVERIFY(object.value("inner").toArray().isEmpty());
    QJsonDocument(object).toBinaryData();
    QCOMPARE(object.value("inner").toArray().size(), 0);
}

void TestQtJson::testObjectNestedEmpty()
{
    QJsonObject object;
    QJsonObject inner;
    QJsonObject inner2;
    object.insert("inner", inner);
    object.insert("inner2", inner2);
    QJsonObject value = object.value("inner").toObject();
    QCOMPARE(value.size(), 0);
    QCOMPARE(value, inner);
    QCOMPARE(value.size(), 0);
    object.insert("count", 0.);
    QCOMPARE(object.value("inner").toObject().size(), 0);
    QCOMPARE(object.value("inner").type(), QJsonValue::Object);
    QJsonDocument(object).toBinaryData();
    QVERIFY(object.value("inner").toObject().isEmpty());
    QVERIFY(object.value("inner2").toObject().isEmpty());
    QJsonDocument doc = QJsonDocument::fromBinaryData(QJsonDocument(object).toBinaryData());
    QVERIFY(!doc.isNull());
    QJsonObject reconstituted(doc.object());
    QCOMPARE(reconstituted.value("inner").toObject().size(), 0);
    QCOMPARE(reconstituted.value("inner").type(), QJsonValue::Object);
    QCOMPARE(reconstituted.value("inner2").type(), QJsonValue::Object);
}

void TestQtJson::testValueRef()
{
    QJsonArray array;
    array.append(1.);
    array.append(2.);
    array.append(3.);
    array[1] = false;

    QCOMPARE(array.size(), 3);
    QCOMPARE(array.at(0).toDouble(), 1.);
    QCOMPARE(array.at(2).toDouble(), 3.);
    QCOMPARE(array.at(1).type(), QJsonValue::Bool);
    QCOMPARE(array.at(1).toBool(), false);

    QJsonObject object;
    object[QLatin1String("key")] = true;
    QCOMPARE(object.size(), 1);
    object.insert(QLatin1String("null"), QJsonValue());
    QCOMPARE(object.value(QLatin1String("null")), QJsonValue());
    object[QLatin1String("null")] = 100.;
    QCOMPARE(object.value(QLatin1String("null")).type(), QJsonValue::Double);
    QJsonValue val = object[QLatin1String("null")];
    QCOMPARE(val.toDouble(), 100.);
    QCOMPARE(object.size(), 2);
}

void TestQtJson::testObjectIteration()
{
    QJsonObject object;
    for (int i = 0; i < 10; ++i)
        object[QString::number(i)] = (double)i;

    QCOMPARE(object.size(), 10);

    for (QJsonObject::iterator it = object.begin(); it != object.end(); ++it) {
        QJsonValue value = it.value();
        QCOMPARE((double)it.key().toInt(), value.toDouble());
    }

    {
        QJsonObject object2 = object;
        QVERIFY(object == object2);

        QJsonValue val = *object2.begin();
        object2.erase(object2.begin());
        QCOMPARE(object.size(), 10);
        QCOMPARE(object2.size(), 9);

        for (QJsonObject::const_iterator it = object2.constBegin(); it != object2.constEnd(); ++it) {
            QJsonValue value = it.value();
            QVERIFY(it.value() != val);
            QCOMPARE((double)it.key().toInt(), value.toDouble());
        }
    }

    {
        QJsonObject::Iterator it = object.begin();
        it += 5;
        QCOMPARE(QJsonValue(it.value()).toDouble(), 5.);
        it -= 3;
        QCOMPARE(QJsonValue(it.value()).toDouble(), 2.);
        QJsonObject::Iterator it2 = it + 5;
        QCOMPARE(QJsonValue(it2.value()).toDouble(), 7.);
        it2 = it - 1;
        QCOMPARE(QJsonValue(it2.value()).toDouble(), 1.);
    }

    {
        QJsonObject::ConstIterator it = object.constBegin();
        it += 5;
        QCOMPARE(QJsonValue(it.value()).toDouble(), 5.);
        it -= 3;
        QCOMPARE(QJsonValue(it.value()).toDouble(), 2.);
        QJsonObject::ConstIterator it2 = it + 5;
        QCOMPARE(QJsonValue(it2.value()).toDouble(), 7.);
        it2 = it - 1;
        QCOMPARE(QJsonValue(it2.value()).toDouble(), 1.);
    }

    QJsonObject::Iterator it = object.begin();
    while (!object.isEmpty())
        it = object.erase(it);
    QCOMPARE(object.size() , 0);
    QVERIFY(it == object.end());
}

void TestQtJson::testArrayIteration()
{
    QJsonArray array;
    for (int i = 0; i < 10; ++i)
        array.append(i);

    QCOMPARE(array.size(), 10);

    int i = 0;
    for (QJsonArray::iterator it = array.begin(); it != array.end(); ++it, ++i) {
        QJsonValue value = (*it);
        QCOMPARE((double)i, value.toDouble());
    }

    {
        QJsonArray array2 = array;
        QVERIFY(array == array2);

        QJsonValue val = *array2.begin();
        array2.erase(array2.begin());
        QCOMPARE(array.size(), 10);
        QCOMPARE(array2.size(), 9);

        i = 1;
        for (QJsonArray::const_iterator it = array2.constBegin(); it != array2.constEnd(); ++it, ++i) {
            QJsonValue value = (*it);
            QCOMPARE((double)i, value.toDouble());
        }
    }

    {
        QJsonArray::Iterator it = array.begin();
        it += 5;
        QCOMPARE(QJsonValue((*it)).toDouble(), 5.);
        it -= 3;
        QCOMPARE(QJsonValue((*it)).toDouble(), 2.);
        QJsonArray::Iterator it2 = it + 5;
        QCOMPARE(QJsonValue(*it2).toDouble(), 7.);
        it2 = it - 1;
        QCOMPARE(QJsonValue(*it2).toDouble(), 1.);
    }

    {
        QJsonArray::ConstIterator it = array.constBegin();
        it += 5;
        QCOMPARE(QJsonValue((*it)).toDouble(), 5.);
        it -= 3;
        QCOMPARE(QJsonValue((*it)).toDouble(), 2.);
        QJsonArray::ConstIterator it2 = it + 5;
        QCOMPARE(QJsonValue(*it2).toDouble(), 7.);
        it2 = it - 1;
        QCOMPARE(QJsonValue(*it2).toDouble(), 1.);
    }

    QJsonArray::Iterator it = array.begin();
    while (!array.isEmpty())
        it = array.erase(it);
    QCOMPARE(array.size() , 0);
    QVERIFY(it == array.end());
}

void TestQtJson::testObjectFind()
{
    QJsonObject object;
    for (int i = 0; i < 10; ++i)
        object[QString::number(i)] = i;

    QCOMPARE(object.size(), 10);

    QJsonObject::iterator it = object.find(QLatin1String("1"));
    QCOMPARE((*it).toDouble(), 1.);
    it = object.find(QLatin1String("11"));
    QVERIFY((*it).type() == QJsonValue::Undefined);
    QVERIFY(it == object.end());

    QJsonObject::const_iterator cit = object.constFind(QLatin1String("1"));
    QCOMPARE((*cit).toDouble(), 1.);
    cit = object.constFind(QLatin1String("11"));
    QVERIFY((*it).type() == QJsonValue::Undefined);
    QVERIFY(it == object.end());
}

void TestQtJson::testDocument()
{
    QJsonDocument doc;
    QCOMPARE(doc.isEmpty(), true);
    QCOMPARE(doc.isArray(), false);
    QCOMPARE(doc.isObject(), false);

    QJsonObject object;
    doc.setObject(object);
    QCOMPARE(doc.isEmpty(), false);
    QCOMPARE(doc.isArray(), false);
    QCOMPARE(doc.isObject(), true);

    object.insert(QLatin1String("Key"), QLatin1String("Value"));
    doc.setObject(object);
    QCOMPARE(doc.isEmpty(), false);
    QCOMPARE(doc.isArray(), false);
    QCOMPARE(doc.isObject(), true);
    QVERIFY(doc.object() == object);
    QVERIFY(doc.array() == QJsonArray());

    doc = QJsonDocument();
    QCOMPARE(doc.isEmpty(), true);
    QCOMPARE(doc.isArray(), false);
    QCOMPARE(doc.isObject(), false);

    QJsonArray array;
    doc.setArray(array);
    QCOMPARE(doc.isEmpty(), false);
    QCOMPARE(doc.isArray(), true);
    QCOMPARE(doc.isObject(), false);

    array.append(QLatin1String("Value"));
    doc.setArray(array);
    QCOMPARE(doc.isEmpty(), false);
    QCOMPARE(doc.isArray(), true);
    QCOMPARE(doc.isObject(), false);
    QVERIFY(doc.array() == array);
    QVERIFY(doc.object() == QJsonObject());

    QJsonObject outer;
    outer.insert(QLatin1String("outerKey"), 22);
    QJsonObject inner;
    inner.insert(QLatin1String("innerKey"), 42);
    outer.insert(QLatin1String("innter"), inner);
    QJsonArray innerArray;
    innerArray.append(23);
    outer.insert(QLatin1String("innterArray"), innerArray);

    QJsonDocument doc2(outer.value(QLatin1String("innter")).toObject());
    QVERIFY(doc2.object().contains(QLatin1String("innerKey")));
    QCOMPARE(doc2.object().value(QLatin1String("innerKey")), QJsonValue(42));

    QJsonDocument doc3;
    doc3.setObject(outer.value(QLatin1String("innter")).toObject());
    QCOMPARE(doc3.isArray(), false);
    QCOMPARE(doc3.isObject(), true);
    QVERIFY(doc3.object().contains(QLatin1String("innerKey")));
    QCOMPARE(doc3.object().value(QLatin1String("innerKey")), QJsonValue(42));

    QJsonDocument doc4(outer.value(QLatin1String("innterArray")).toArray());
    QCOMPARE(doc4.isArray(), true);
    QCOMPARE(doc4.isObject(), false);
    QCOMPARE(doc4.array().size(), 1);
    QCOMPARE(doc4.array().at(0), QJsonValue(23));

    QJsonDocument doc5;
    doc5.setArray(outer.value(QLatin1String("innterArray")).toArray());
    QCOMPARE(doc5.isArray(), true);
    QCOMPARE(doc5.isObject(), false);
    QCOMPARE(doc5.array().size(), 1);
    QCOMPARE(doc5.array().at(0), QJsonValue(23));
}

void TestQtJson::nullValues()
{
    QJsonArray array;
    array.append(QJsonValue());

    QCOMPARE(array.size(), 1);
    QCOMPARE(array.at(0), QJsonValue());

    QJsonObject object;
    object.insert(QString("key"), QJsonValue());
    QCOMPARE(object.contains("key"), true);
    QCOMPARE(object.size(), 1);
    QCOMPARE(object.value("key"), QJsonValue());
}

void TestQtJson::nullArrays()
{
    QJsonArray nullArray;
    QJsonArray nonNull;
    nonNull.append(QLatin1String("bar"));

    QCOMPARE(nullArray, QJsonArray());
    QVERIFY(nullArray != nonNull);
    QVERIFY(nonNull != nullArray);

    QCOMPARE(nullArray.size(), 0);
    QCOMPARE(nullArray.takeAt(0), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullArray.first(), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullArray.last(), QJsonValue(QJsonValue::Undefined));
    nullArray.removeAt(0);
    nullArray.removeAt(-1);

    nullArray.append(QString("bar"));
    nullArray.removeAt(0);

    QCOMPARE(nullArray.size(), 0);
    QCOMPARE(nullArray.takeAt(0), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullArray.first(), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullArray.last(), QJsonValue(QJsonValue::Undefined));
    nullArray.removeAt(0);
    nullArray.removeAt(-1);
}

void TestQtJson::nullObject()
{
    QJsonObject nullObject;
    QJsonObject nonNull;
    nonNull.insert(QLatin1String("foo"), QLatin1String("bar"));

    QCOMPARE(nullObject, QJsonObject());
    QVERIFY(nullObject != nonNull);
    QVERIFY(nonNull != nullObject);

    QCOMPARE(nullObject.size(), 0);
    QCOMPARE(nullObject.keys(), QStringList());
    nullObject.remove("foo");
    QCOMPARE(nullObject, QJsonObject());
    QCOMPARE(nullObject.take("foo"), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullObject.contains("foo"), false);

    nullObject.insert("foo", QString("bar"));
    nullObject.remove("foo");

    QCOMPARE(nullObject.size(), 0);
    QCOMPARE(nullObject.keys(), QStringList());
    nullObject.remove("foo");
    QCOMPARE(nullObject, QJsonObject());
    QCOMPARE(nullObject.take("foo"), QJsonValue(QJsonValue::Undefined));
    QCOMPARE(nullObject.contains("foo"), false);
}

void TestQtJson::keySorting()
{
    const char *json = "{ \"B\": true, \"A\": false }";
    QJsonDocument doc = QJsonDocument::fromJson(json);

    QCOMPARE(doc.isObject(), true);

    QJsonObject o = doc.object();
    QCOMPARE(o.size(), 2);
    QJsonObject::const_iterator it = o.constBegin();
    QCOMPARE(it.key(), QLatin1String("A"));
    ++it;
    QCOMPARE(it.key(), QLatin1String("B"));
}

void TestQtJson::undefinedValues()
{
    QJsonObject object;
    object.insert("Key", QJsonValue(QJsonValue::Undefined));
    QCOMPARE(object.size(), 0);

    object.insert("Key", QLatin1String("Value"));
    QCOMPARE(object.size(), 1);
    QCOMPARE(object.value("Key").type(), QJsonValue::String);
    QCOMPARE(object.value("foo").type(), QJsonValue::Undefined);
    object.insert("Key", QJsonValue(QJsonValue::Undefined));
    QCOMPARE(object.size(), 0);
    QCOMPARE(object.value("Key").type(), QJsonValue::Undefined);

    QJsonArray array;
    array.append(QJsonValue(QJsonValue::Undefined));
    QCOMPARE(array.size(), 1);
    QCOMPARE(array.at(0).type(), QJsonValue::Null);

    QCOMPARE(array.at(1).type(), QJsonValue::Undefined);
    QCOMPARE(array.at(-1).type(), QJsonValue::Undefined);
}


void TestQtJson::fromVariantMap()
{
    QVariantMap map;
    map.insert(QLatin1String("key1"), QLatin1String("value1"));
    map.insert(QLatin1String("key2"), QLatin1String("value2"));
    QJsonObject object = QJsonObject::fromVariantMap(map);
    QCOMPARE(object.size(), 2);
    QCOMPARE(object.value(QLatin1String("key1")), QJsonValue(QLatin1String("value1")));
    QCOMPARE(object.value(QLatin1String("key2")), QJsonValue(QLatin1String("value2")));

    QVariantList list;
    list.append(true);
    list.append(QVariant());
    list.append(999.);
    list.append(QLatin1String("foo"));
    map.insert("list", list);
    object = QJsonObject::fromVariantMap(map);
    QCOMPARE(object.size(), 3);
    QCOMPARE(object.value(QLatin1String("key1")), QJsonValue(QLatin1String("value1")));
    QCOMPARE(object.value(QLatin1String("key2")), QJsonValue(QLatin1String("value2")));
    QCOMPARE(object.value(QLatin1String("list")).type(), QJsonValue::Array);
    QJsonArray array = object.value(QLatin1String("list")).toArray();
    QCOMPARE(array.size(), 4);
    QCOMPARE(array.at(0).type(), QJsonValue::Bool);
    QCOMPARE(array.at(0).toBool(), true);
    QCOMPARE(array.at(1).type(), QJsonValue::Null);
    QCOMPARE(array.at(2).type(), QJsonValue::Double);
    QCOMPARE(array.at(2).toDouble(), 999.);
    QCOMPARE(array.at(3).type(), QJsonValue::String);
    QCOMPARE(array.at(3).toString(), QLatin1String("foo"));
}

void TestQtJson::toVariantMap()
{
    QJsonObject object;
    object.insert("Key", QString("Value"));
    object.insert("null", QJsonValue());
    QJsonArray array;
    array.append(true);
    array.append(999.);
    array.append(QLatin1String("string"));
    array.append(QJsonValue());
    object.insert("Array", array);

    QVariantMap map = object.toVariantMap();

    QCOMPARE(map.size(), 3);
    QCOMPARE(map.value("Key"), QVariant(QString("Value")));
    QCOMPARE(map.value("null"), QVariant());
    QCOMPARE(map.value("Array").type(), QVariant::List);
    QVariantList list = map.value("Array").toList();
    QCOMPARE(list.size(), 4);
    QCOMPARE(list.at(0), QVariant(true));
    QCOMPARE(list.at(1), QVariant(999.));
    QCOMPARE(list.at(2), QVariant(QLatin1String("string")));
    QCOMPARE(list.at(3), QVariant());
}

void TestQtJson::toJson()
{
    QJsonObject object;
    object.insert("\\Key\n", QString("Value"));
    object.insert("null", QJsonValue());
    QJsonArray array;
    array.append(true);
    array.append(999.);
    array.append(QLatin1String("string"));
    array.append(QJsonValue());
    array.append(QLatin1String("\\\a\n\r\b\tabcABC\""));
    object.insert("Array", array);

    QByteArray json = QJsonDocument(object).toJson();

    QByteArray expected =
            "{\n"
            "    \"Array\": [\n"
            "        true,\n"
            "        999,\n"
            "        \"string\",\n"
            "        null,\n"
            "        \"\\\\\\u0007\\n\\r\\b\\tabcABC\\\"\"\n"
            "    ],\n"
            "    \"\\\\Key\\n\": \"Value\",\n"
            "    \"null\": null\n"
            "}\n";
    QCOMPARE(json, expected);

    QJsonDocument doc;
    doc.setObject(object);
    json = doc.toJson();
    QCOMPARE(json, expected);

    doc.setArray(array);
    json = doc.toJson();
    expected =
            "[\n"
            "    true,\n"
            "    999,\n"
            "    \"string\",\n"
            "    null,\n"
            "    \"\\\\\\u0007\\n\\r\\b\\tabcABC\\\"\"\n"
            "]\n";
    QCOMPARE(json, expected);
}

void TestQtJson::fromJson()
{
    {
        QByteArray json = "[\n    true\n]\n";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), true);
        QCOMPARE(doc.isObject(), false);
        QJsonArray array = doc.array();
        QCOMPARE(array.size(), 1);
        QCOMPARE(array.at(0).type(), QJsonValue::Bool);
        QCOMPARE(array.at(0).toBool(), true);
        QCOMPARE(doc.toJson(), json);
    }
    {
        QByteArray json = "[]";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), true);
        QCOMPARE(doc.isObject(), false);
        QJsonArray array = doc.array();
        QCOMPARE(array.size(), 0);
    }
    {
        QByteArray json = "{}";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), false);
        QCOMPARE(doc.isObject(), true);
        QJsonObject object = doc.object();
        QCOMPARE(object.size(), 0);
    }
    {
        QByteArray json = "{\n    \"Key\": true\n}\n";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), false);
        QCOMPARE(doc.isObject(), true);
        QJsonObject object = doc.object();
        QCOMPARE(object.size(), 1);
        QCOMPARE(object.value("Key"), QJsonValue(true));
        QCOMPARE(doc.toJson(), json);
    }
    {
        QByteArray json = "[ null, true, false, \"Foo\", 1, [], {} ]";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), true);
        QCOMPARE(doc.isObject(), false);
        QJsonArray array = doc.array();
        QCOMPARE(array.size(), 7);
        QCOMPARE(array.at(0).type(), QJsonValue::Null);
        QCOMPARE(array.at(1).type(), QJsonValue::Bool);
        QCOMPARE(array.at(1).toBool(), true);
        QCOMPARE(array.at(2).type(), QJsonValue::Bool);
        QCOMPARE(array.at(2).toBool(), false);
        QCOMPARE(array.at(3).type(), QJsonValue::String);
        QCOMPARE(array.at(3).toString(), QLatin1String("Foo"));
        QCOMPARE(array.at(4).type(), QJsonValue::Double);
        QCOMPARE(array.at(4).toDouble(), 1.);
        QCOMPARE(array.at(5).type(), QJsonValue::Array);
        QCOMPARE(array.at(5).toArray().size(), 0);
        QCOMPARE(array.at(6).type(), QJsonValue::Object);
        QCOMPARE(array.at(6).toObject().size(), 0);
    }
    {
        QByteArray json = "{ \"0\": null, \"1\": true, \"2\": false, \"3\": \"Foo\", \"4\": 1, \"5\": [], \"6\": {} }";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), false);
        QCOMPARE(doc.isObject(), true);
        QJsonObject object = doc.object();
        QCOMPARE(object.size(), 7);
        QCOMPARE(object.value("0").type(), QJsonValue::Null);
        QCOMPARE(object.value("1").type(), QJsonValue::Bool);
        QCOMPARE(object.value("1").toBool(), true);
        QCOMPARE(object.value("2").type(), QJsonValue::Bool);
        QCOMPARE(object.value("2").toBool(), false);
        QCOMPARE(object.value("3").type(), QJsonValue::String);
        QCOMPARE(object.value("3").toString(), QLatin1String("Foo"));
        QCOMPARE(object.value("4").type(), QJsonValue::Double);
        QCOMPARE(object.value("4").toDouble(), 1.);
        QCOMPARE(object.value("5").type(), QJsonValue::Array);
        QCOMPARE(object.value("5").toArray().size(), 0);
        QCOMPARE(object.value("6").type(), QJsonValue::Object);
        QCOMPARE(object.value("6").toObject().size(), 0);
    }
}

void TestQtJson::fromBinary()
{
    QFile file(QLatin1String(SRCDIR "test.json"));
    file.open(QFile::ReadOnly);
    QByteArray testJson = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(testJson);
    QJsonDocument outdoc = QJsonDocument::fromBinaryData(doc.toBinaryData());
    QVERIFY(!outdoc.isNull());
    QVERIFY(doc == outdoc);

    QFile bfile(QLatin1String(SRCDIR "test.bjson"));
    bfile.open(QFile::ReadOnly);
    QByteArray binary = bfile.readAll();

    QJsonDocument bdoc = QJsonDocument::fromBinaryData(binary);
    QVERIFY(!bdoc.isNull());
    QVERIFY(doc.toVariant() == bdoc.toVariant());
    QVERIFY(doc == bdoc);
}

void TestQtJson::toAndFromBinary_data()
{
    QTest::addColumn<QString>("filename");
    QTest::newRow("test.json") << QString::fromLatin1(SRCDIR "test.json");
    QTest::newRow("test2.json") << QString::fromLatin1(SRCDIR "test2.json");
}

void TestQtJson::toAndFromBinary()
{
    QFETCH(QString, filename);
    QFile file(filename);
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QVERIFY(!doc.isNull());
    QJsonDocument outdoc = QJsonDocument::fromBinaryData(doc.toBinaryData());
    QVERIFY(!outdoc.isNull());
    QVERIFY(doc == outdoc);
}

void TestQtJson::parseNumbers()
{
    {
        // test number parsing
        struct Numbers {
            const char *str;
            int n;
        };
        Numbers numbers [] = {
            { "0", 0 },
            { "1", 1 },
            { "10", 10 },
            { "-1", -1 },
            { "100000", 100000 },
            { "-999", -999 }
        };
        int size = sizeof(numbers)/sizeof(Numbers);
        for (int i = 0; i < size; ++i) {
            QByteArray json = "[ ";
            json += numbers[i].str;
            json += " ]";
            QJsonDocument doc = QJsonDocument::fromJson(json);
            QVERIFY(!doc.isEmpty());
            QCOMPARE(doc.isArray(), true);
            QCOMPARE(doc.isObject(), false);
            QJsonArray array = doc.array();
            QCOMPARE(array.size(), 1);
            QJsonValue val = array.at(0);
            QCOMPARE(val.type(), QJsonValue::Double);
            QCOMPARE(val.toDouble(), (double)numbers[i].n);
        }
    }
    {
        // test number parsing
        struct Numbers {
            const char *str;
            double n;
        };
        Numbers numbers [] = {
            { "0", 0 },
            { "1", 1 },
            { "10", 10 },
            { "-1", -1 },
            { "100000", 100000 },
            { "-999", -999 },
            { "1.1", 1.1 },
            { "1e10", 1e10 },
            { "-1.1", -1.1 },
            { "-1e10", -1e10 },
            { "-1E10", -1e10 },
            { "1.1e10", 1.1e10 },
            { "1.1e308", 1.1e308 },
            { "-1.1e308", -1.1e308 },
            { "1.1e-308", 1.1e-308 },
            { "-1.1e-308", -1.1e-308 },
            { "1.1e+308", 1.1e+308 },
            { "-1.1e+308", -1.1e+308 },
            { "1.e+308", 1.e+308 },
            { "-1.e+308", -1.e+308 }
        };
        int size = sizeof(numbers)/sizeof(Numbers);
        for (int i = 0; i < size; ++i) {
            QByteArray json = "[ ";
            json += numbers[i].str;
            json += " ]";
            QJsonDocument doc = QJsonDocument::fromJson(json);
            QVERIFY(!doc.isEmpty());
            QCOMPARE(doc.isArray(), true);
            QCOMPARE(doc.isObject(), false);
            QJsonArray array = doc.array();
            QCOMPARE(array.size(), 1);
            QJsonValue val = array.at(0);
            QCOMPARE(val.type(), QJsonValue::Double);
            QCOMPARE(val.toDouble(), numbers[i].n);
        }
    }
}

void TestQtJson::parseStrings()
{
    const char *strings [] =
    {
        "Foo",
        "abc\\\"abc",
        "abc\\\\abc",
        "abc\\babc",
        "abc\\fabc",
        "abc\\nabc",
        "abc\\rabc",
        "abc\\tabc",
        "abc\\u0019abc",
        "abcЂabc",
    };
    int size = sizeof(strings)/sizeof(const char *);

    for (int i = 0; i < size; ++i) {
        QByteArray json = "[\n    \"";
        json += strings[i];
        json += "\"\n]\n";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), true);
        QCOMPARE(doc.isObject(), false);
        QJsonArray array = doc.array();
        QCOMPARE(array.size(), 1);
        QJsonValue val = array.at(0);
        QCOMPARE(val.type(), QJsonValue::String);

        QCOMPARE(doc.toJson(), json);
    }

    struct Pairs {
        const char *in;
        const char *out;
    };
    Pairs pairs [] = {
        { "abc\\/abc", "abc/abc" },
        { "abc\\u0402abc", "abcЂabc" },
        { "abc\\u0065abc", "abceabc" }
    };
    size = sizeof(pairs)/sizeof(Pairs);

    for (int i = 0; i < size; ++i) {
        QByteArray json = "[\n    \"";
        json += pairs[i].in;
        json += "\"\n]\n";
        QByteArray out = "[\n    \"";
        out += pairs[i].out;
        out += "\"\n]\n";
        QJsonDocument doc = QJsonDocument::fromJson(json);
        QVERIFY(!doc.isEmpty());
        QCOMPARE(doc.isArray(), true);
        QCOMPARE(doc.isObject(), false);
        QJsonArray array = doc.array();
        QCOMPARE(array.size(), 1);
        QJsonValue val = array.at(0);
        QCOMPARE(val.type(), QJsonValue::String);

        QCOMPARE(doc.toJson(), out);
    }

}

void TestQtJson::parseDuplicateKeys()
{
    const char *json = "{ \"B\": true, \"A\": null, \"B\": false }";

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QCOMPARE(doc.isObject(), true);

    QJsonObject o = doc.object();
    QCOMPARE(o.size(), 2);
    QJsonObject::const_iterator it = o.constBegin();
    QCOMPARE(it.key(), QLatin1String("A"));
    QCOMPARE(it.value(), QJsonValue());
    ++it;
    QCOMPARE(it.key(), QLatin1String("B"));
    QCOMPARE(it.value(), QJsonValue(false));
}

void TestQtJson::testParser()
{
    QFile file(QLatin1String(SRCDIR "test.json"));
    file.open(QFile::ReadOnly);
    QByteArray testJson = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(testJson);
    QVERIFY(!doc.isEmpty());
}

void TestQtJson::compactArray()
{
    QJsonArray array;
    array.append(QLatin1String("First Entry"));
    array.append(QLatin1String("Second Entry"));
    array.append(QLatin1String("Third Entry"));
    QJsonDocument doc(array);
    int s =  doc.toBinaryData().size();
    array.removeAt(1);
    doc.setArray(array);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("[\n"
                        "    \"First Entry\",\n"
                        "    \"Third Entry\"\n"
                        "]\n"));

    array.removeAt(0);
    doc.setArray(array);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("[\n"
                        "    \"Third Entry\"\n"
                        "]\n"));

    array.removeAt(0);
    doc.setArray(array);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("[\n"
                        "]\n"));

}

void TestQtJson::compactObject()
{
    QJsonObject object;
    object.insert(QLatin1String("Key1"), QLatin1String("First Entry"));
    object.insert(QLatin1String("Key2"), QLatin1String("Second Entry"));
    object.insert(QLatin1String("Key3"), QLatin1String("Third Entry"));
    QJsonDocument doc(object);
    int s =  doc.toBinaryData().size();
    object.remove(QLatin1String("Key2"));
    doc.setObject(object);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("{\n"
                        "    \"Key1\": \"First Entry\",\n"
                        "    \"Key3\": \"Third Entry\"\n"
                        "}\n"));

    object.remove(QLatin1String("Key1"));
    doc.setObject(object);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("{\n"
                        "    \"Key3\": \"Third Entry\"\n"
                        "}\n"));

    object.remove(QLatin1String("Key3"));
    doc.setObject(object);
    QVERIFY(s > doc.toBinaryData().size());
    s = doc.toBinaryData().size();
    QCOMPARE(doc.toJson(),
             QByteArray("{\n"
                        "}\n"));

}

void TestQtJson::validation()
{
    // this basically tests that we don't crash on corrupt data
    QFile file(QLatin1String(SRCDIR "test.json"));
    QVERIFY(file.open(QFile::ReadOnly));
    QByteArray testJson = file.readAll();
    QVERIFY(!testJson.isEmpty());

    QJsonDocument doc = QJsonDocument::fromJson(testJson);

    QByteArray binary = doc.toBinaryData();

    // only test the first 1000 bytes. Testing the full file takes too long
    for (int i = 0; i < 1000; ++i) {
        QByteArray corrupted = binary;
        corrupted[i] = 0xff;
        QJsonDocument doc = QJsonDocument::fromBinaryData(corrupted);
        if (doc.isNull())
            continue;
        QByteArray json = doc.toJson();
    }


    QFile file2(QLatin1String(SRCDIR "foo.json"));
//    QVERIFY(file2.open(QFile::ReadOnly)); // ### the file is missing o_O
    file2.open(QFile::ReadOnly);
    testJson = file2.readAll();
//    QVERIFY(!testJson.isEmpty());

    doc = QJsonDocument::fromJson(testJson);

    binary = doc.toBinaryData();

    for (int i = 0; i < binary.size(); ++i) {
        QByteArray corrupted = binary;
        corrupted[i] = 0xff;
        QJsonDocument doc = QJsonDocument::fromBinaryData(corrupted);
        if (doc.isNull())
            continue;
        QByteArray json = doc.toJson();

        corrupted = binary;
        corrupted[i] = 0x00;
        doc = QJsonDocument::fromBinaryData(corrupted);
        if (doc.isNull())
            continue;
        json = doc.toJson();
    }
}

void TestQtJson::assignToDocument()
{
    {
        const char *json = "{ \"inner\": { \"key\": true } }";
        QJsonDocument doc = QJsonDocument::fromJson(json);

        QJsonObject o = doc.object();
        QJsonValue inner = o.value("inner");

        QJsonDocument innerDoc(inner.toObject());

        QVERIFY(innerDoc != doc);
        QVERIFY(innerDoc.object() == inner.toObject());
    }
    {
        const char *json = "[ [ true ] ]";
        QJsonDocument doc = QJsonDocument::fromJson(json);

        QJsonArray a = doc.array();
        QJsonValue inner = a.at(0);

        QJsonDocument innerDoc(inner.toArray());

        QVERIFY(innerDoc != doc);
        QVERIFY(innerDoc.array() == inner.toArray());
    }
}


void TestQtJson::testDuplicateKeys()
{
    QJsonObject obj;
    obj.insert(QLatin1String("foo"), QLatin1String("bar"));
    obj.insert(QLatin1String("foo"), QLatin1String("zap"));
    QCOMPARE(obj.size(), 1);
    QCOMPARE(obj.value(QLatin1String("foo")).toString(), QLatin1String("zap"));
}

void TestQtJson::testCompaction()
{
    // modify object enough times to trigger compactionCounter
    // and make sure the data is still valid
    QJsonObject obj;
    for (int i = 0; i < 33; ++i) {
        obj.remove(QLatin1String("foo"));
        obj.insert(QLatin1String("foo"), QLatin1String("bar"));
    }
    QCOMPARE(obj.size(), 1);
    QCOMPARE(obj.value(QLatin1String("foo")).toString(), QLatin1String("bar"));

    QJsonDocument doc = QJsonDocument::fromBinaryData(QJsonDocument(obj).toBinaryData());
    QVERIFY(!doc.isNull());
    QVERIFY(!doc.isEmpty());
    QCOMPARE(doc.isArray(), false);
    QCOMPARE(doc.isObject(), true);
    QVERIFY(doc.object() == obj);
}

void TestQtJson::testDebugStream()
{
    {
        // QJsonObject

        QJsonObject object;
        QTest::ignoreMessage(QtDebugMsg, "QJsonObject() ");
        qDebug() << object;

        object.insert(QLatin1String("foo"), QLatin1String("bar"));
        QTest::ignoreMessage(QtDebugMsg, "QJsonObject({\"foo\": \"bar\"}) ");
        qDebug() << object;
    }

    {
        // QJsonArray

        QJsonArray array;
        QTest::ignoreMessage(QtDebugMsg, "QJsonArray() ");
        qDebug() << array;

        array.append(1);
        array.append(QLatin1String("foo"));
        QTest::ignoreMessage(QtDebugMsg, "QJsonArray([1,\"foo\"]) ");
        qDebug() << array;
    }

    {
        // QJsonDocument

        QJsonDocument doc;
        QTest::ignoreMessage(QtDebugMsg, "QJsonDocument() ");
        qDebug() << doc;

        QJsonObject object;
        object.insert(QLatin1String("foo"), QLatin1String("bar"));
        doc.setObject(object);
        QTest::ignoreMessage(QtDebugMsg, "QJsonDocument({\"foo\": \"bar\"}) ");
        qDebug() << doc;

        QJsonArray array;
        array.append(1);
        array.append(QLatin1String("foo"));
        QTest::ignoreMessage(QtDebugMsg, "QJsonDocument([1,\"foo\"]) ");
        doc.setArray(array);
        qDebug() << doc;
    }

    {
        // QJsonValue

        QJsonValue value;

        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(null) ");
        qDebug() << value;

        value = QJsonValue(true); // bool
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(bool, true) ");
        qDebug() << value;

        value = QJsonValue((double)4.2); // double
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(double, 4.2) ");
        qDebug() << value;

        value = QJsonValue((int)42); // int
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(double, 42) ");
        qDebug() << value;

        value = QJsonValue(QLatin1String("foo")); // string
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(string, \"foo\") ");
        qDebug() << value;

        QJsonArray array;
        array.append(1);
        array.append(QLatin1String("foo"));
        value = QJsonValue(array); // array
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(array, QJsonArray([1,\"foo\"]) ) ");
        qDebug() << value;

        QJsonObject object;
        object.insert(QLatin1String("foo"), QLatin1String("bar"));
        value = QJsonValue(object); // object
        QTest::ignoreMessage(QtDebugMsg, "QJsonValue(object, QJsonObject({\"foo\": \"bar\"}) ) ");
        qDebug() << value;
    }
}

void TestQtJson::testCompactionError()
{
    QJsonObject schemaObject;
    schemaObject.insert("_Type", QLatin1String("_SchemaType"));
    schemaObject.insert("name", QLatin1String("Address"));
    schemaObject.insert("schema", QJsonObject());
    {
        QJsonObject content(schemaObject);
        QJsonDocument doc(content);
        QVERIFY(!doc.isNull());
        QByteArray hash = QCryptographicHash::hash(doc.toBinaryData(), QCryptographicHash::Md5).toHex();
        schemaObject.insert("_Version", QString::fromLatin1(hash.constData(), hash.size()));
    }

    QJsonObject schema;
    schema.insert("streetNumber", schema.value("number").toObject());
    schemaObject.insert("schema", schema);
    {
        QJsonObject content(schemaObject);
        content.remove("_Uuid");
        content.remove("_Version");
        QJsonDocument doc(content);
        QVERIFY(!doc.isNull());
        QByteArray hash = QCryptographicHash::hash(doc.toBinaryData(), QCryptographicHash::Md5).toHex();
        schemaObject.insert("_Version", QString::fromLatin1(hash.constData(), hash.size()));
    }
}

QTEST_MAIN(TestQtJson)
#include "tst_qtjson.moc"
