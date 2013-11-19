#pragma once
#include <string>
#include <dlfcn.h>
#include <boost/function.hpp>

template<class T>
class CPluginFactory
{
protected:
    CPluginFactory()
    {
        m_libraryHandle = NULL;
    }

    virtual ~CPluginFactory()
    {
        BOOST_ASSERT(m_libraryHandle == NULL);
    }

    void * GetFunctionPointer(const std::string & funcName)
    {
        if (m_libraryHandle != NULL) 
        {
            return dlsym(m_libraryHandle, funcName.c_str());	
        }
        return NULL;
    }



public:
    virtual bool load(const std::string & libraryFile)
    {
        bool result = FALSE;

        m_libraryHandle = dlopen(libraryFile.c_str(), RTLD_LAZY);
        if (m_libraryHandle != NULL) 
        {
            m_construct = (T* (*)(void)) dlsym(m_libraryHandle, "construct");
            m_destruct = (void (*)(T*)) dlsym(m_libraryHandle, "destruct");
        }
        else
        {
            std::cerr << "Plugin: " << dlerror() << std::endl;
        }

        if(m_construct !=NULL && m_destruct!=NULL)
            result = TRUE;

        return result;
    }

    void unload()
    {
        if(m_libraryHandle != NULL)
        {
            dlclose(m_libraryHandle);
            m_libraryHandle = NULL;
        }
    }


    T * construct()
    {
        if(m_construct != NULL)
            return m_construct();
        return NULL;
    }

    void destruct(T* item)
    {
        if(m_destruct != NULL)
            m_destruct(item);
    }


private:
    void * m_libraryHandle;
    boost::function<T* ()> m_construct;
    boost::function<void (T*)> m_destruct;
    
};


