#ifndef QFUTUREBUILDER_H
#define QFUTUREBUILDER_H

#include <QFuture>

#include <type_traits>

template <typename T, typename S>
struct Then
{
};

template <typename S>
struct Then<void, S>
{
};

template <typename T>
class QFutureBuilder : public QFutureInterface<T>
{
public:
    template<typename Continuation>
    inline QFutureBuilder<typename std::result_of<Continuation(T)>::type> then(Continuation c)
    {
        return QFutureBuilder<typename std::result_of<Continuation(T)>::type>();
    }

    operator QFuture<T>()
    {
        return QFuture<T>(new QFutureBuilder<T>());
    }
};

template <typename T>
QFutureBuilder<T> qFuture(T value)
{
}

template <typename T>
QFutureBuilder<T> qFuture(QFuture<T> &&future)
{
}

#endif // QFUTUREBUILDER_H
