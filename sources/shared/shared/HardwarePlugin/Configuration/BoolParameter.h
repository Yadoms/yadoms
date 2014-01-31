#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

//--------------------------------------------------------------
/// \class Hardware plugin configuration bool parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationBoolParameter : public CHardwarePluginConfigurationSimpleParameter<bool>
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] defaultValue  Parameter default value
   //--------------------------------------------------------------
   CHardwarePluginConfigurationBoolParameter(const std::string& name, const std::string& description, bool defaultValue);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfigurationBoolParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   virtual const std::string valueToString(bool current) const;
   virtual void valueFromString(const std::string& valueAsString);
   // [END] CHardwarePluginConfigurationParameter implementation
};
