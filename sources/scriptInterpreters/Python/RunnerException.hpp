#pragma once

#include <shared/exception/Exception.hpp>
#include "PythonBorrowedObject.h"

//--------------------------------------------------------------
/// \class Exception for Python script runner
//--------------------------------------------------------------
class CRunnerException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CRunnerException(const std::string& message)
      :CException(message)
   {
      CPythonBorrowedObject pyError(PyErr_Occurred());
      if (pyError.isNull())
         return;

      PyObject* pyErrorType = NULL;
      PyObject* pyErrorMessage = NULL;
      PyObject* pyTraceBack = NULL;
      PyErr_Fetch(&pyErrorType, &pyErrorMessage, &pyTraceBack);

      Py_ssize_t nbMessages = PyTuple_Size(pyErrorMessage);
      if (nbMessages > 0)
         m_message += " : ";
      for (Py_ssize_t i=0; i<nbMessages; ++i)
      {
         CPythonBorrowedObject message(PyTuple_GetItem(pyErrorMessage, i));
         if (message.isNull() || PyString_AsString(*message) == NULL)
            continue;
         m_message += std::string(PyString_AsString(*message)) + "\n";
      }
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CRunnerException() throw()
   {
   }
};
