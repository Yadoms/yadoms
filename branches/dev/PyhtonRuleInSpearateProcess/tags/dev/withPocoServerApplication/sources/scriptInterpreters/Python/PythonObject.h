#pragma once
#include "PythonLibInclude.h"

//--------------------------------------------------------------
/// \brief	Python object, implementing RAII for PyObject type
//--------------------------------------------------------------
class CPythonObject
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPythonObject();

   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \parm[in] object Newly created Python object
   //--------------------------------------------------------------
   CPythonObject(PyObject* object);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPythonObject();

   //--------------------------------------------------------------
   /// \brief	Reset the pointer
   /// \parm[in] object Newly created Python object, or NULL
   /// \note Currently old pointer is deleted (Python reference count is decremented)
   //--------------------------------------------------------------
   void reset(PyObject* object = NULL);

   //--------------------------------------------------------------
   /// \brief	Copy operator
   //--------------------------------------------------------------
   CPythonObject& operator=(const CPythonObject& other);

   //--------------------------------------------------------------
   /// \brief	Operator *, returning the Python object
   //--------------------------------------------------------------
   PyObject* operator*() const;

   //--------------------------------------------------------------
   /// \brief	Another method to access Python object
   //--------------------------------------------------------------
   PyObject* get() const;

   //--------------------------------------------------------------
   /// \brief	Check if Python object is null
   /// \return true if null
   //--------------------------------------------------------------
   bool isNull() const;

private:
   //--------------------------------------------------------------
   /// \brief	The Python interpreter initializer
   //--------------------------------------------------------------
   PyObject* m_object;
};



