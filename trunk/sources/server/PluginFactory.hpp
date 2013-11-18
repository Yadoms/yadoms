#ifndef __PLUGIN_LOADER_H__
#define __PLUGIN_LOADER_H__

#include <string>

#ifdef WIN32
#include "Windows.h"
#else
#include <dlfcn.h>
#endif

#include <boost/function.hpp>


template<class T, class TInfo>
class CPluginFactory
{
public:
    CPluginFactory()
    {
        m_libraryHandle = NULL;
    }

    ~CPluginFactory()
    {
        BOOST_ASSERT(m_libraryHandle == NULL);
    }


    bool load(const std::string & libraryFile)
    {
        bool result = FALSE;

#ifdef WIN32
        m_libraryHandle = LoadLibrary(libraryFile.c_str());
        if (m_libraryHandle != NULL) 
        {
            // use the result in a call to GetProcAddress
            m_construct = (T* (*)(void)) GetProcAddress(m_libraryHandle, "construct");		   
            m_destruct = (void (*)(T*)) GetProcAddress(m_libraryHandle, "destruct");
            m_getInfo = (TInfo* (*)(void)) GetProcAddress(m_libraryHandle, "getInfo");		   
        } 
        else
        {
            std::cerr << "Plugin: " << GetLastError() << std::endl;
        }
#else
        m_libraryHandle = dlopen(libraryFile.c_str(), RTLD_LAZY);
        if (m_libraryHandle != NULL) 
        {
            m_construct = (T* (*)(void)) dlsym(m_libraryHandle, "construct");
            m_destruct = (void (*)(T*)) dlsym(m_libraryHandle, "destruct");
            m_getInfo = (TInfo* (*)(void)) dlsym(m_libraryHandle, "getInfo");		   
        }
        else
        {
            std::cerr << "Plugin: " << dlerror() << std::endl;
        }
#endif	
        if(m_construct !=NULL && m_destruct!=NULL && m_getInfo != NULL)
            result = TRUE;

        return result;
    }

    void unload()
    {
        if(m_libraryHandle != NULL)
        {
#ifdef WIN32
            FreeLibrary(m_libraryHandle);
#else
            dlclose(m_libraryHandle);
#endif		
            m_libraryHandle = NULL;
        }
    }


    T * generate()
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

    TInfo * getInfo()
    {
        if(m_getInfo != NULL)
            return m_getInfo();
        return NULL;
    }

private:
#ifdef WIN32
    HMODULE m_libraryHandle;
#else
    void * m_libraryHandle;
#endif

    boost::function<T* ()> m_construct;
    boost::function<void (T*)> m_destruct;
    boost::function<TInfo* ()> m_getInfo;
    
};



#endif //!defined(__PLUGIN_LOADER_H__)