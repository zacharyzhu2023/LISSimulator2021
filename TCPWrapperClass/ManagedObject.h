/*
ManagedObject: generic template class to create an instance of a C++/CLI wrapper
to convert unmanaged C++ code to use from C#.
*/
#pragma once
#include <msclr\marshal_cppstd.h>

using namespace System;

template<class T>
public ref class ManagedObject
{
protected:
    T* m_Instance;
public:
    ManagedObject(T* instance)
        : m_Instance(instance)
    {
    }
    virtual ~ManagedObject()
    {
        if (m_Instance != nullptr)
        {
            delete m_Instance;
        }
    }
    !ManagedObject()
    {
        if (m_Instance != nullptr)
        {
            delete m_Instance;
        }
    }
    T* GetInstance()
    {
        return m_Instance;
    }
};

static const std::string toCPPString(String^ inp)
{
    std::string output = msclr::interop::marshal_as<std::string>(inp);
    return output;
}

static String^ toCSString(std::string inp)
{
    String^ output = gcnew String(inp.c_str());
    return output;
}