#pragma once

#include <shared/Export.h>
#include "Parameter.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration string parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationStringParameter : public CHardwarePluginConfigurationParameter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] defaultValue  Parameter default value
   //--------------------------------------------------------------
   CHardwarePluginConfigurationStringParameter(const std::string& name, const std::string& description, const std::string& defaultValue);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfigurationStringParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   virtual const std::string valueToString(bool current) const;
   virtual void valueFromString(const std::string& valueAsString);
   // [END] CHardwarePluginConfigurationParameter implementation

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
};