#pragma once

#include <shared/Export.h>
#include "EnumGenericParameter.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration serial port parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationSerialPortParameter : public CHardwarePluginConfigurationEnumGeneric
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] defaultValue  Parameter default value
   //--------------------------------------------------------------
   CHardwarePluginConfigurationSerialPortParameter(const std::string& name, const std::string& description, const std::string& defaultValue);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   ~CHardwarePluginConfigurationSerialPortParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   virtual const std::string valueToString(bool current) const;
   virtual void valueFromString(const std::string& valueAsString);
   // [END] CHardwarePluginConfigurationParameter implementation

   // CHardwarePluginConfigurationEnumGeneric override
   virtual void getSchema(boost::property_tree::ptree& pt) const;
   // [END] CHardwarePluginConfigurationEnumGeneric override

   //--------------------------------------------------------------
   /// \brief	    Native getter
   /// \return    The current parameter value
   //--------------------------------------------------------------
   const std::string& get() const;

private:
   //--------------------------------------------------------------
   /// \brief	    The default parameter value
   //--------------------------------------------------------------
   const std::string m_defaultValue;

   //--------------------------------------------------------------
   /// \brief	    The current parameter value
   //--------------------------------------------------------------
   std::string m_value;

   //--------------------------------------------------------------
   /// \brief	    The current parameter value
   //--------------------------------------------------------------
   const std::vector<std::string> m_valueNames;
};
