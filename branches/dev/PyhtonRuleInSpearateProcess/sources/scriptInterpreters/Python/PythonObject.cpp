#include "stdafx.h"
#include "PythonObject.h"

CPythonObject::CPythonObject()
   :m_object(NULL)
{
}

CPythonObject::CPythonObject(PyObject* object)
   :m_object(object)
{
}

CPythonObject::~CPythonObject()
{
   Py_XDECREF(m_object);
}

void CPythonObject::reset(PyObject* object)
{
   Py_XDECREF(m_object);
   m_object = object;
}

CPythonObject& CPythonObject::operator=(const CPythonObject& other)
{
   if (this == &other)
      return *this;

   reset(*other);
   return *this;
}

PyObject* CPythonObject::operator*() const
{
   return m_object;
}

PyObject* CPythonObject::get() const
{
   return m_object;
}

bool CPythonObject::isNull() const
{
   return m_object == NULL;
}