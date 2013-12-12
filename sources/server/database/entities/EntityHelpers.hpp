#pragma once

//
/// \brief Macro used to declare a private field with public getter and setter
//
#define DELCARE_FIELD(fieldType, fieldName) \
   public:\
      const fieldType get##fieldName() const {return m_##fieldName; } \
      void set##fieldName(const fieldType newValue) { m_##fieldName = newValue; } \
   private:\
      fieldType m_##fieldName;

