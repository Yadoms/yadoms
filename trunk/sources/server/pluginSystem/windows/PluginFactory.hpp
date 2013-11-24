//
// PluginFactory.hpp
//
// yadoms-plugin factory (base)
//
#pragma once

#include <string>
#include "Windows.h"
#include <boost/function.hpp>

//--------------------------------------------------------------
/// \brief	This class is used to load a plugin file library and 
///         provide construct and destruct methods
//--------------------------------------------------------------
template<class T>
class CPluginFactory
{
protected:
    //--------------------------------------------------------------
    /// \brief	Constructor
    //--------------------------------------------------------------
    CPluginFactory()
    {
        m_libraryHandle = NULL;
    }

    //--------------------------------------------------------------
    /// \brief	Destructor
    //--------------------------------------------------------------
    virtual ~CPluginFactory()
    {
        BOOST_ASSERT(m_libraryHandle == NULL);
    }
    
    
    //--------------------------------------------------------------
    /// \brief	    Get a function pointer
    /// \param [in] funcName: the exported function to search
    /// \return     a function pointer or NULL if file is not loaded or funtion is not found
    //-------------------------------------------------------------
    void * GetFunctionPointer(const std::string & funcName)
    {
        if (m_libraryHandle != NULL) 
        {
            return GetProcAddress(m_libraryHandle, funcName.c_str());	
        }
        return NULL;
    }


public:
    //--------------------------------------------------------------
    /// \brief	    Loads a library file
    /// \param [in] libraryFile: the library file path
    /// \return     true if loaded with success, else false
    //-------------------------------------------------------------
    virtual bool load(const std::string & libraryFile)//TODO : passer en protected ?
    {
        bool result = FALSE;

        //load library
        m_libraryHandle = LoadLibrary(libraryFile.c_str());

        if (m_libraryHandle != NULL) 
        {
            //load construct and destruct exported functions
            m_construct = (T* (*)(void))GetFunctionPointer("construct");
            m_destruct =  (void (*)(T*))GetFunctionPointer("destruct"); 

            //check that construct and destruct are correctly loaded
            if(m_construct !=NULL && m_destruct!=NULL)
            {
                result = TRUE;
            }
            else
            {
                //library is not compatible (do not have construct and destruct exporeted functions)
                //free memory
                unload();
            }
        } 
        else
        {
            std::cerr << "Plugin: " << GetLastError() << std::endl;
        }

        return result;
    }

    //--------------------------------------------------------------
    /// \brief	    Free library file
    //-------------------------------------------------------------
    void unload()//TODO : passer en protected ?
    {
        if(m_libraryHandle != NULL)
        {
            FreeLibrary(m_libraryHandle);
            m_libraryHandle = NULL;
        }
    }

    //--------------------------------------------------------------
    /// \brief	    Construct a plugin instance (call the contruct library method)
    /// \return     a new plugin instance
    //-------------------------------------------------------------
    T * construct()//TODO : redescendre hors des classes platform-specific
    {
        if(m_construct != NULL)
            return m_construct();
        return NULL;
    }

    //--------------------------------------------------------------
    /// \brief	    Destruct a plugin instance
    /// \param [in] item: the plugin to destruct
    //-------------------------------------------------------------
    void destruct(T* item)//TODO : redescendre hors des classes platform-specific
    {
        if(m_destruct != NULL)
            m_destruct(item);
    }


private:
    //-------------------------------------------------------------
    /// \brief	    The library handle
    //-------------------------------------------------------------
    HMODULE m_libraryHandle;

    //-------------------------------------------------------------
    /// \brief	    Function pointer to "construct" exported function
    //-------------------------------------------------------------
    boost::function<T* ()> m_construct;
    //-------------------------------------------------------------
    /// \brief	    Function pointer to "destruct" exported function
    //-------------------------------------------------------------
    boost::function<void (T*)> m_destruct;
    
};

