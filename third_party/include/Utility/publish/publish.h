#pragma once
#include <mutex>
#include <map>
#include <algorithm>
#include <vector>
#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>

namespace utility
{
    template <typename T>
    class Publish
    {
    public:
        using Subscribe = typename T;
    public:
        Publish() = default;
        ~Publish() = default;

    public:
        void startSubscribe(Subscribe* subscribe) noexcept
        {
            std::lock_guard<std::recursive_mutex> locker(m_subscribeMutex);
            m_subcribeList.push_back(subscribe);
        }
        void stopSubscribe(Subscribe* subscribe) noexcept
        {
            std::lock_guard<std::recursive_mutex> locker(m_subscribeMutex);
            auto pos = std::find(m_subcribeList.begin(), m_subcribeList.end(), subscribe);
            if (pos != m_subcribeList.end())
            {
                *pos = nullptr;
            }
        }

        template <typename Fun>
        void foreachSubscribe(Fun f)
        {
            std::lock_guard<std::recursive_mutex> locker(m_subscribeMutex);
            for (auto it = m_subcribeList.begin(); it != m_subcribeList.end();)
            {
                if (*it == nullptr)
                {
                    it = m_subcribeList.erase(it);
                }
                else
                {
                    f(*it);
                    ++it;
                }
            }
        }

    private:
        std::recursive_mutex m_subscribeMutex;
        std::list<Subscribe*> m_subcribeList;
    };


    template<>
    class Publish<QObject>
    {
    public:
        using Subscribe = QObject;
    public:
        Publish(QObject* obj) noexcept
            :m_object(obj)
        {

        }
        ~Publish()
        {

        }


    public:
        void startSubscribe(QObject* subscribe) noexcept
        {
            std::call_once(m_onceFlag, &Publish::init, this);
            for (auto& singal_slot : m_signalSlotNames)
            {
                if (subscribe->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(singal_slot.second.data() + 1)) != -1)
                {
                    QObject::connect(m_object, singal_slot.first.data(), subscribe, singal_slot.second.data(), Qt::DirectConnection);
                }
            }
            auto method = getMethod(subscribe, "onSubscribed");
            if (method.isValid() && !method.parameterTypes().empty())
            {
                method.invoke(subscribe, Qt::DirectConnection, QGenericArgument(method.parameterTypes().front().data(), &m_object));
            }
        }

        void stopSubscribe(QObject* subscribe) noexcept
        {
            m_object->disconnect(subscribe);
        }


    private:
        void init() noexcept
        {
            const QMetaObject* obj = m_object->metaObject();
            for (auto i = QObject::staticMetaObject.methodCount(); i < obj->methodCount(); ++i)
            {
                auto mathod = obj->method(i);
                if (mathod.methodType() == QMetaMethod::MethodType::Signal)
                {
                    QString singal = mathod.name() + "(";
                    for (int i = 0; i < mathod.parameterTypes().size(); ++i)
                    {
                        singal.append(mathod.parameterTypes()[i]);
                        if (i < mathod.parameterTypes().size() - 1)
                        {
                            singal.append(",");
                        }
                    }
                    singal.append(")");
                    auto upper_singal = singal;
                    upper_singal.front() = upper_singal.front().toUpper();
                    std::string singal_name = "2" + singal.toStdString();
                    std::string slot_name = "1on" + upper_singal.toStdString();
                    m_signalSlotNames.push_back(std::pair(singal_name, slot_name));
                }
            }
        }

        QMetaMethod getMethod(QObject* obj, const QByteArray& method_name) noexcept
        {
            for (int i = 0; i < obj->metaObject()->methodCount(); ++i)
            {
                QMetaMethod d = obj->metaObject()->method(i);
                if (d.name() == method_name)
                {
                    return d;
                }
            }
            return QMetaMethod();
        }

        bool checkType(const QByteArray& type) noexcept
        {
            return true;
        }

    private:
        QObject* m_object;
        std::once_flag m_onceFlag;
        std::vector<std::pair<std::string, std::string>> m_signalSlotNames;
    };
}
