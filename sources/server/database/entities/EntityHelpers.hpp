#pragma once

//
/// \brief Macro used to declare a private field with public getter and setter
//
#define DELCARE_FIELD(className, fieldType, fieldName) \
   public:\
      const fieldType get##fieldName() const {return m_##fieldName; } \
      className& set##fieldName(const fieldType newValue) { m_##fieldName = newValue; return *this; } \
   private:\
      fieldType m_##fieldName;

