#pragma once
#include <pthread.h>
#include <memory>
#include <iostream>
#include <Log/Log.h>

#define MUTEX_TPDCLR() template <typename DataType>

MUTEX_TPDCLR()
class Mutex
{

private:

    pthread_mutex_t _Mutex = PTHREAD_MUTEX_INITIALIZER;
    std::unique_ptr<DataType> Data;    

protected:

    virtual void PostLock(){};
    virtual void PreLock(){};
    virtual void PostUnlock(){};
    virtual void PreUnlock(){};

public:

    Mutex();
    ~Mutex();
    void Lock();
    void Unlock();
    DataType* GetData();
    DataType* operator->();
    void operator=(const DataType &Data);
    DataType operator+(const DataType & Data);
    DataType operator-(const DataType & Data);

};

MUTEX_TPDCLR() Mutex<DataType>::Mutex()
{
    Data = std::unique_ptr<DataType>(new DataType());
};

MUTEX_TPDCLR() Mutex<DataType>::~Mutex(){};

MUTEX_TPDCLR() void Mutex<DataType>::Lock()
{
    PreLock();
    if(pthread_mutex_trylock(&_Mutex) != 0)
    {
        Log::Warning("Recurso %x já está trancado, aguardando liberação...", Data.get());
        pthread_mutex_lock(&_Mutex);
    }
    PostLock();
    Log::Resource("Recurso %x trancado pela thread %lu", Data.get(), pthread_self());
}

MUTEX_TPDCLR() void Mutex<DataType>::Unlock()
{
    PreUnlock();
    pthread_mutex_unlock(&_Mutex);
    PostUnlock();
    Log::Resource("Recurso %x destrancado pela thread %lu", Data.get(), pthread_self());
}

MUTEX_TPDCLR() DataType* Mutex<DataType>::GetData()
{
    return this->Data.get();
}

MUTEX_TPDCLR() DataType* Mutex<DataType>::operator->()
{
    return Data;
}

MUTEX_TPDCLR() void Mutex<DataType>::operator=(const DataType &Data)
{
    *(this->Data.get()) = Data;
}

MUTEX_TPDCLR() DataType Mutex<DataType>:: operator+(const DataType & Data)
{
    return DataType(*(this->Data.get()) + Data);
}

MUTEX_TPDCLR() DataType Mutex<DataType>::operator-(const DataType & Data)
{
    return DataType(*(this->Data.get()) - Data);
}