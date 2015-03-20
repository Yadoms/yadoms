#pragma once

#include <shared/exception/Exception.hpp>
#include "PythonObject.h"
#include "PythonBorrowedObject.h"

//--------------------------------------------------------------
/// \class Exception raised by Python error
//--------------------------------------------------------------
class CPythonException : public shared::exception::CException
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   //--------------------------------------------------------------
   CPythonException(const std::string& message)
      :CException(message)
   {
      CPythonBorrowedObject pyError(PyErr_Occurred());
      if (pyError.isNull())
         return;

      PyObject* pyErrorType = NULL;
      PyObject* pyErrorMessage = NULL;
      PyObject* pyTraceBack = NULL;
      PyErr_Fetch(&pyErrorType, &pyErrorMessage, &pyTraceBack);
      PyErr_Clear();

      if (pyErrorType)
      {
         CPythonObject type(PyObject_Str(pyErrorType));
         m_message += PyString_AsString(*type);
      }
      if (pyErrorMessage)
      {
         CPythonObject message(PyObject_Str(pyErrorMessage));
         m_message += ": ";
         m_message += PyString_AsString(*message);
      }
      if (pyTraceBack)
      {
         CPythonObject traceBack(PyObject_Str(pyTraceBack));
         m_message += ", ";
         m_message += PyString_AsString(*traceBack);
      }

      Py_XDECREF(pyErrorType);
      Py_XDECREF(pyErrorMessage);
      Py_XDECREF(pyTraceBack);
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CPythonException() throw()
   {
   }
};
