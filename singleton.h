#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

template <typename T1>
class Singleton
{
public:
    static std::shared_ptr<T1> getInstance()
    {
        if(mInstance == nullptr)
            mInstance = std::make_shared<T1>();

        return mInstance;
    }

    template <typename T2>
    static std::shared_ptr<T1> getInstance(T2 _param)
    {
        if(mInstance == nullptr)
            mInstance = std::make_shared<T1>(_param);

        return mInstance;
    }
private:
    Singleton() {}
    static::std::shared_ptr<T1> mInstance;
};

template <typename T1>
std::shared_ptr<T1> Singleton<T1>::mInstance = nullptr;

#endif // SINGLETON_H
