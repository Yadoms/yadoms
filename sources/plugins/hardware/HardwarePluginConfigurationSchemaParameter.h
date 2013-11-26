#pragma once

#include <string>

//--------------------------------------------------------------
/// \brief		Configuration parameter description
//--------------------------------------------------------------
struct CHardwarePluginConfigurationSchemaParameter
{
   enum EParameterType
   {
      kBool,         /// \brief  Boolean parameter
      kEnum,         /// \brief  Enum parameter
      kInt,          /// \brief  Integer parameter
      kDouble,       /// \brief  Floating point parameter
      kString,       /// \brief  String parameter
      kDate          /// \brief  Date parameter
   };

   std::string m_name;
   EParameterType m_type;
   std::string m_defaultValue;
   std::string m_enumValues;
};
