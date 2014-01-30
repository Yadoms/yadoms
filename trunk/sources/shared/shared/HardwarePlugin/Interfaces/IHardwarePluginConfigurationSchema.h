#pragma once

#include <shared/Export.h>

//--------------------------------------------------------------
/// \class Hardware plugin configuration schema
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT IHardwarePluginConfigurationSchema
{
public:
   ~IHardwarePluginConfigurationSchema() {}

   //--------------------------------------------------------------
   /// \brief	    Get the full configuration description as string
   ///             The description contains parameters name, description, values, etc...
   /// \return     A string containing all the configuration description (JSON format)
   //--------------------------------------------------------------
   virtual std::string getSchema() const = 0;
};

