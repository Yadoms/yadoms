#pragma once
#include "PythonLibInclude.h"

//--------------------------------------------------------------
/// \brief	Python borrowed referenced object, implementing RAII
/// \note This class is just provided to have homogeneous code,
/// it is not very useful (does not provided big added value)
//--------------------------------------------------------------
class CPythonBorrowedObject
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPythonBorrowedObject();

   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \parm[in] object Newly created Python object
   //--------------------------------------------------------------
   CPythonBorrowedObject(PyObject* object);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPythonBorrowedObject();

   //--------------------------------------------------------------
   /// \brief	Reset the pointer
   /// \parm[in] object Newly created Python object, or NULL
   /// \note Currently old pointer is deleted (Python reference count is decremented)
   //--------------------------------------------------------------
   void reset(PyObject* object = NULL);

   //--------------------------------------------------------------
   /// \brief	Copy operator
   //--------------------------------------------------------------
   CPythonBorrowedObject& operator=(const CPythonBorrowedObject& other);

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



