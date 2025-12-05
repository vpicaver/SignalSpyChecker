//Our includes
#include "SignalSpyChecker.h"

//Catch includes
#include <catch2/catch_test_macros.hpp>

//Qt includes
#include <QDebug>

using namespace SignalSpyChecker;

Optional::Optional()
{

}

Optional::Optional(const std::initializer_list<std::pair<SignalSpy *, QList<int> > > &list) :
    QHash<SignalSpy*, QList<int>>(list)
{

}


void Optional::checkSpies() const
{
    checkHelper([](bool okay) {
        CHECK(okay);
    });

}

void Optional::requireSpies() const
{
    checkHelper([](bool okay) {
        REQUIRE(okay);
    });
}

void Optional::clearSpyCounts()
{
    for(auto iter = begin(); iter != end(); iter++) {
        iter.key()->clear();
        iter.value().clear();
    }
}


void Optional::checkHelper(std::function<void (bool)> checkFunc) const
{
    for(auto iter = begin(); iter != end(); iter++) {
        INFO("Key:" << iter.key()->objectName().toStdString());
        bool okay = false;
        QString expectedStr;
        for(int i = 0; i < iter.value().size(); i++) {
            auto expected = iter.value().at(i);
            expectedStr += QString("%1").arg(expected);

            if(i < iter.value().size() - 1) {
                expectedStr += " or ";
            }

            if(iter.key()->size() == expected) {
                okay = true;
            }
        }

        if(!okay)  {
            qDebug() << "Spy checker will fail. Place breakpoint here to debug";
        }

        INFO("SignalSpy:" << iter.key()->size() << " expected:" << expectedStr.toStdString());
        checkFunc(okay);
    }
}


Constant::Constant(const std::initializer_list<std::pair<SignalSpy *, int> > &list) :
    QHash<SignalSpy*, int>(list)
{

}

void Constant::checkSpies() const
{
    checkHelper([](bool okay) {
        CHECK(okay);
    });
}

void Constant::requireSpies() const
{
    checkHelper([](bool okay) {
        REQUIRE(okay);
    });
}

void Constant::clearSpyCounts()
{
    for(auto iter = begin(); iter != end(); iter++) {
        iter.key()->clear();
        iter.value() = 0;
    }
}

Constant Constant::makeChecker(QObject *object)
{
    Constant checker;

    auto metaObject = object->metaObject();
    for(int i = 0; i < metaObject->methodCount(); i++) {
        auto method = metaObject->method(i);
        if(method.methodType() == QMetaMethod::Signal) {
            QString spyName = method.name() + QStringLiteral("Spy");
            bool exist = [&]() {
                for(const auto& keyValue : checker.asKeyValueRange()) {
                    if(keyValue.first->objectName() == spyName) {
                        return true;
                    }
                }
                return false;
            }();

            if(!exist) {
                SignalSpy* spy = new SignalSpy(object, method);
                spy->setObjectName(spyName);

                checker.insert(spy, 0);
            }
        }
    }

    return checker;
}

void Constant::checkHelper(std::function<void (bool)> checkFunc) const
{
    for(auto iter = begin(); iter != end(); iter++) {
        INFO("Key:" << iter.key()->objectName().toStdString());
        if(iter.key()->size() != iter.value()) {
            qDebug() << "Spy checker will fail. Place breakpoint here to debug. checkSpies()";
        }
        INFO("SignalSpy:" << iter.key()->size() << " expected:" << iter.value());
        checkFunc(iter.key()->size() == iter.value());

    }
}
