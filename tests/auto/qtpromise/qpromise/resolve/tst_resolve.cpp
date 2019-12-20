/*
 * Copyright (c) 2019 Simon Brunel, https://github.com/simonbrunel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "../../shared/data.h"
#include "../../shared/utils.h"

// QtPromise
#include <QtPromise>

// Qt
#include <QtTest>

// STL
#include <memory>

using namespace QtPromise;

class tst_qpromise_resolve : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void value();
    void noValue();
    void moveRValue();
    void copyLValue();
    void qtSharedPtr();
    void stdSharedPtr();
};

QTEST_MAIN(tst_qpromise_resolve)
#include "tst_resolve.moc"

void tst_qpromise_resolve::value()
{
    int v0 = 42;
    const int v1 = 42;

    auto p0 = QPromise<int>::resolve(42);
    auto p1 = QPromise<int>::resolve(v0);
    auto p2 = QPromise<int>::resolve(v1);

    Q_STATIC_ASSERT((std::is_same<decltype(p0), QPromise<int>>::value));
    Q_STATIC_ASSERT((std::is_same<decltype(p1), QPromise<int>>::value));
    Q_STATIC_ASSERT((std::is_same<decltype(p2), QPromise<int>>::value));

    for (const auto& p : {p0, p1, p2}) {
        QCOMPARE(p.isFulfilled(), true);
    }
    for (const auto& p : {p0, p1, p2}) {
        QCOMPARE(waitForValue(p, -1), 42);
    }
}

void tst_qpromise_resolve::noValue()
{
    auto p = QPromise<void>::resolve();

    Q_STATIC_ASSERT((std::is_same<decltype(p), QPromise<void>>::value));

    QCOMPARE(p.isFulfilled(), true);
    QCOMPARE(waitForValue(p, -1, 42), 42);
}

void tst_qpromise_resolve::moveRValue()
{
    Data::logs().reset();

    {
        auto p = QtPromise::resolve(Data(42)).wait();

        Q_STATIC_ASSERT((std::is_same<decltype(p), QPromise<Data>>::value));
    }

    QCOMPARE(Data::logs().ctor, 1);
    QCOMPARE(Data::logs().copy, 0);
    QCOMPARE(Data::logs().move, 1);
    QCOMPARE(Data::logs().refs, 0);
}

void tst_qpromise_resolve::copyLValue()
{
    Data::logs().reset();

    {
        Data value(42);
        auto p = QtPromise::resolve(value).wait();

        Q_STATIC_ASSERT((std::is_same<decltype(p), QPromise<Data>>::value));
    }

    QCOMPARE(Data::logs().ctor, 1);
    QCOMPARE(Data::logs().copy, 1);
    QCOMPARE(Data::logs().move, 0);
    QCOMPARE(Data::logs().refs, 0);
}

// https://github.com/simonbrunel/qtpromise/issues/6
void tst_qpromise_resolve::qtSharedPtr()
{
    Data::logs().reset();

    QWeakPointer<Data> wptr;

    {
        QSharedPointer<Data> sptr0(new Data(42));
        const QSharedPointer<Data> sptr1 = sptr0;

        auto p0 = QPromise<QSharedPointer<Data>>::resolve(QSharedPointer<Data>(new Data(42)));
        auto p1 = QPromise<QSharedPointer<Data>>::resolve(sptr0);
        auto p2 = QPromise<QSharedPointer<Data>>::resolve(sptr1);

        Q_STATIC_ASSERT((std::is_same<decltype(p0), QPromise<QSharedPointer<Data>>>::value));
        Q_STATIC_ASSERT((std::is_same<decltype(p1), QPromise<QSharedPointer<Data>>>::value));
        Q_STATIC_ASSERT((std::is_same<decltype(p2), QPromise<QSharedPointer<Data>>>::value));

        QCOMPARE(waitForValue(p1, QSharedPointer<Data>()), sptr0);
        QCOMPARE(waitForValue(p2, QSharedPointer<Data>()), sptr1);

        wptr = sptr0;

        QCOMPARE(wptr.isNull(), false);
        QCOMPARE(Data::logs().refs, 2);
    }

    QCOMPARE(wptr.isNull(), true);

    QCOMPARE(Data::logs().ctor, 2);
    QCOMPARE(Data::logs().copy, 0);
    QCOMPARE(Data::logs().move, 0);
    QCOMPARE(Data::logs().refs, 0);
}

// https://github.com/simonbrunel/qtpromise/issues/6
void tst_qpromise_resolve::stdSharedPtr()
{
    Data::logs().reset();

    std::weak_ptr<Data> wptr;

    {
        std::shared_ptr<Data> sptr0(new Data(42));
        const std::shared_ptr<Data> sptr1 = sptr0;

        auto p0 = QPromise<std::shared_ptr<Data>>::resolve(std::shared_ptr<Data>(new Data(42)));
        auto p1 = QPromise<std::shared_ptr<Data>>::resolve(sptr0);
        auto p2 = QPromise<std::shared_ptr<Data>>::resolve(sptr1);

        Q_STATIC_ASSERT((std::is_same<decltype(p0), QPromise<std::shared_ptr<Data>>>::value));
        Q_STATIC_ASSERT((std::is_same<decltype(p1), QPromise<std::shared_ptr<Data>>>::value));
        Q_STATIC_ASSERT((std::is_same<decltype(p2), QPromise<std::shared_ptr<Data>>>::value));

        QCOMPARE(waitForValue(p1, std::shared_ptr<Data>()), sptr0);
        QCOMPARE(waitForValue(p2, std::shared_ptr<Data>()), sptr1);

        wptr = sptr0;

        QCOMPARE(wptr.use_count(), 4l);
        QCOMPARE(Data::logs().refs, 2);
    }

    QCOMPARE(wptr.use_count(), 0l);

    QCOMPARE(Data::logs().ctor, 2);
    QCOMPARE(Data::logs().copy, 0);
    QCOMPARE(Data::logs().move, 0);
    QCOMPARE(Data::logs().refs, 0);
}
