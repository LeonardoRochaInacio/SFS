#pragma once
#include <pthread.h>
#include <Log/Log.h>
#include <errno.h>
#include <iostream>

typedef void * (*ThreadFunctionPointer)(void *);

#define THREADABLE_TPDCLR() template<typename ReturnType>

THREADABLE_TPDCLR()
class Threadable
{
protected:

    bool ThreadCreated = false;
    pthread_t ThreadID;

private:

    ReturnType LastThreadReturnValue;

public:

    void ThreadStart();
    void ThreadCancel();
    void ThreadJoin();
    ReturnType GetThreadReturn();

protected:

    Threadable();
    ~Threadable();
    virtual ReturnType ThreadRun() = 0;

private:

    static void * ThreadEntryPoint(void * Args);

};

THREADABLE_TPDCLR() void Threadable<ReturnType>::ThreadStart()
{
    int Result = pthread_create(&ThreadID, nullptr, reinterpret_cast<ThreadFunctionPointer>(Threadable::ThreadEntryPoint), this);
    switch(Result)
    {
        case EAGAIN:
            Log::Error("Recursos insuficientes para criar outra thread.");
            break;
        case EINVAL:
            Log::Error("Atributes inválidos.");
            break;
        case EPERM:
            Log::Error("Falta de permissão para manipular attributes");
            break;
        case 0:
            Log::Success("Thread %lu criada com sucesso!", (long int)ThreadID);
            ThreadCreated = true;
            break;
    }    
}

THREADABLE_TPDCLR() void Threadable<ReturnType>::ThreadCancel()
{
    Log::Resource("Encerrando thread %lu", ThreadID);
    ThreadCreated = false;
    pthread_cancel(ThreadID);
    pthread_exit(0);
}

THREADABLE_TPDCLR() void Threadable<ReturnType>::ThreadJoin()
{
    Log::Resource("Thread %lu joined", ThreadID);
    pthread_join(ThreadID, nullptr);
};

THREADABLE_TPDCLR() ReturnType Threadable<ReturnType>::GetThreadReturn()
{
    return LastThreadReturnValue;
}

THREADABLE_TPDCLR() Threadable<ReturnType>::Threadable(){};

THREADABLE_TPDCLR() Threadable<ReturnType>::~Threadable() {};

THREADABLE_TPDCLR() void * Threadable<ReturnType>::ThreadEntryPoint(void * Args)
{
    Threadable* ThreadObj = static_cast<Threadable*>(Args);
    ThreadObj->LastThreadReturnValue = ThreadObj->ThreadRun();
    ThreadObj->ThreadCancel();
    return nullptr;
};