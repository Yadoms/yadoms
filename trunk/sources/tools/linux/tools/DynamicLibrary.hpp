//TODO : !!! à coder voir windows
#pragma once
#include <string>
#include <dlfcn.h>
#include <boost/function.hpp>
#include <boost/log/trivial.hpp>
#include "../../tools/Log.h"

//--------------------------------------------------------------
/// \brief	This class is used to load a plugin file library and 
///         provide construct and destruct methods
//--------------------------------------------------------------
class CDynamicLibrary
{
protected:
    //--------------------------------------------------------------
    /// \brief	Constructor
    //--------------------------------------------------------------
    CDynamicLibrary()
    {
        m_libraryHandle = NULL;
    }

    //--------------------------------------------------------------
    /// \brief	Destructor
    //--------------------------------------------------------------
    virtual ~CDynamicLibrary()
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
            return dlsym(m_libraryHandle, funcName.c_str());	
        }
        return NULL;
    }



    //--------------------------------------------------------------
    /// \brief	    Loads a library file
    /// \param [in] libraryFile: the library file path
    /// \return     true if loaded with success, else false
    //-------------------------------------------------------------
    virtual bool load(const std::string & libraryFile)
    {
        bool result = false;

        //load library
        m_libraryHandle = dlopen(libraryFile.c_str(), RTLD_LAZY);

        if (m_libraryHandle != NULL) 
        {
            result = true;
        } 
        else
        {
			   YADOMS_LOG(error) << "Fail to load library : " << libraryFile << dlerror();
        }

        return result;
    }

    //--------------------------------------------------------------
    /// \brief	    Free library file
    //-------------------------------------------------------------
    void unload()
    {
        if(m_libraryHandle != NULL)
        {
            dlclose(m_libraryHandle);
            m_libraryHandle = NULL;
        }
    }

private:
    //-------------------------------------------------------------
    /// \brief	    The library handle
    //-------------------------------------------------------------
    void * m_libraryHandle;
};


