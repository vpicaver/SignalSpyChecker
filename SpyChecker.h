#ifndef SPYCHECKER_H
#define SPYCHECKER_H

//Qt includes
#include <QHash>
#include <QSignalSpy>
#include <QMetaMethod>

//Std includes
#include <initializer_list>

namespace SignalSpyChecker {

class SignalSpy : public QSignalSpy
{
public:
    explicit SignalSpy(const QObject *obj, const char *aSignal)
        : QSignalSpy(obj, aSignal)  {}

    template <typename Func>
    SignalSpy(const typename QtPrivate::FunctionPointer<Func>::Object *obj, Func signal0)
        : QSignalSpy(obj,signal0) {}

    SignalSpy(const QObject *obj, QMetaMethod signal)
        : QSignalSpy(obj, signal) {}

    void setObjectName(const QString& name) {
        m_objectName = name;
    }

    QString objectName() const {
        return m_objectName;
    }


private:
    QString m_objectName;

};

class Optional : public QHash<SignalSpy*, QList<int>>
{
public:
    Optional();
    //Send a list of valid values for expected value
    Optional(const std::initializer_list<std::pair<SignalSpy*, QList<int>>>& list);

    void checkSpies() const;
    void requireSpies() const;
    void clearSpyCounts();

    template<typename Signal>
    QSignalSpy* findSpy(Signal signal) const {
        auto keys = this->keys();
        auto method = QMetaMethod::fromSignal(signal);
        auto iter = std::find_if(keys.begin(), keys.end(), [method](const QSignalSpy* spy) {
            return spy->signal() == method.methodSignature();
        });
        if(iter != keys.end()) {
            return *iter;
        }
        Q_ASSERT(false);
        return nullptr;
    }

private:
    void checkHelper(std::function<void (bool)> checkFunc) const;
};

class Constant : public QHash<SignalSpy*, int>
{
public:
    Constant() {}
    //Send a list of valid values for expected value
    Constant(const std::initializer_list<std::pair<SignalSpy*, int>>& list);

    void checkSpies() const;
    void requireSpies() const;
    void clearSpyCounts();

    template<typename Signal>
    SignalSpy* findSpy(Signal signal) const {
        auto keys = this->keys();
        auto method = QMetaMethod::fromSignal(signal);
        auto iter = std::find_if(keys.begin(), keys.end(), [method](const SignalSpy* spy) {
            return spy->signal() == method.methodSignature();
        });
        if(iter != keys.end()) {
            return *iter;
        }
        Q_ASSERT(false);
        return nullptr;
    }


    static Constant makeChecker(QObject* object);

private:
    void checkHelper(std::function<void (bool)> checkFunc) const;
};

};

#endif // SPYCHECKER_H
