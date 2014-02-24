#pragma once

//
/// \brief Macro used to declare a private field with public getter and setter
//
#define DELCARE_FIELD(className, fieldType, fieldName) \
   public:\
      const fieldType get##fieldName() const {return m_##fieldName; } \
      const bool is##fieldName##Filled() const {return m_##fieldName##_filled; } \
      className& set##fieldName(const fieldType newValue) { m_##fieldName = newValue; m_##fieldName##_filled = true; return *this; } \
   private:\
      fieldType m_##fieldName;\
      bool m_##fieldName##_filled;

