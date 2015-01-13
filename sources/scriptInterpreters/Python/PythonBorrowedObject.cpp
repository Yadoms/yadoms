#include "stdafx.h"
#include "PythonBorrowedObject.h"

CPythonBorrowedObject::CPythonBorrowedObject()
   :m_object(NULL)
{
}

CPythonBorrowedObject::CPythonBorrowedObject(PyObject* object)
   :m_object(object)
{
}

CPythonBorrowedObject::~CPythonBorrowedObject()
{
}

void CPythonBorrowedObject::reset(PyObject* object)
{
   m_object = object;
}

CPythonBorrowedObject& CPythonBorrowedObject::operator=(const CPythonBorrowedObject& other)
{
   if (this == &other)
      return *this;

   reset(*other);
   return *this;
}

PyObject* CPythonBorrowedObject::operator*() const
{
   return m_object;
}

PyObject* CPythonBorrowedObject::get() const
{
   return m_object;
}

bool CPythonBorrowedObject::isNull() const
{
   return m_object == NULL;
}