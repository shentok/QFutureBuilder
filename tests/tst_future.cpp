#include <QtTest/QTest>

// QtPromise
#include <QFutureBuilder>

#include <QObject>
#include <QString>

class tst_future : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void then();
    void then_void();
    void fail();
    void fail_void();
}; // class tst_future

void tst_future::then()
{
    QString result;
    auto input = qFuture(42);
    auto continuation = [](int res) -> int { return res; };
    QFuture<int> output = input.then(continuation);

    QCOMPARE(output.isStarted(), true);

    output.waitForFinished();

    QCOMPARE(output.isFinished(), true);
    QCOMPARE(output.result(), 42);
}

#if 0
void tst_future::then_void()
{
    QString result;
    auto input = qPromise();
    auto output = input.then([&]() {
        return QtConcurrent::run([&]() {
            result = "foo";
        });
    });

    QCOMPARE(input.isFulfilled(), true);
    QCOMPARE(output.isPending(), true);

    output.then([&]() {
        result += "bar";
    }).wait();

    QCOMPARE(input.isFulfilled(), true);
    QCOMPARE(result, QString("foobar"));
}

void tst_future::fail()
{
    QString result;
    auto input = QPromise<QString>::reject(MyException("bar"));
    auto output = input.fail([](const MyException& e) {
        return QtConcurrent::run([](const QString& error) {
            return QString("foo%1").arg(error);
        }, e.error());
    });

    QCOMPARE(input.isRejected(), true);
    QCOMPARE(output.isPending(), true);

    output.then([&](const QString& res) {
        result = res;
    }).wait();

    QCOMPARE(output.isFulfilled(), true);
    QCOMPARE(result, QString("foobar"));
}

void tst_future::fail_void()
{
    QString result;
    auto input = QPromise<void>::reject(MyException("bar"));
    auto output = input.fail([&](const MyException& e) {
        return QtConcurrent::run([&](const QString& error) {
            result = error;
        }, e.error());
    });

    QCOMPARE(input.isRejected(), true);
    QCOMPARE(output.isPending(), true);

    output.then([&]() {
        result = result.prepend("foo");
    }).wait();

    QCOMPARE(output.isFulfilled(), true);
    QCOMPARE(result, QString("foobar"));
}

void tst_future::finally()
{
    auto input = qPromise(42);
    auto output = input.finally([]() {
        return QtConcurrent::run([]() {
            return QString("foo");
        });
    });

    Q_STATIC_ASSERT((std::is_same<decltype(output), QPromise<int>>::value));

    QCOMPARE(input.isFulfilled(), true);
    QCOMPARE(output.isPending(), true);

    int value = -1;
    output.then([&](int res) {
        value = res;
    }).wait();

    QCOMPARE(output.isFulfilled(), true);
    QCOMPARE(value, 42);
}

void tst_future::finallyRejected()
{
    auto input = qPromise(42);
    auto output = input.finally([]() {
        return QtConcurrent::run([]() {
            throw MyException("foo");
        });
    });

    Q_STATIC_ASSERT((std::is_same<decltype(output), QPromise<int>>::value));

    QCOMPARE(input.isFulfilled(), true);
    QCOMPARE(output.isPending(), true);

    QString error;
    output.fail([&](const MyException& e) {
        error = e.error();
        return -1;
    }).wait();

    QCOMPARE(output.isRejected(), true);
    QCOMPARE(error, QString("foo"));
}
#endif

QTEST_MAIN(tst_future)
#include "tst_future.moc"
