#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

//--------------------------------------------------------------
/// \type Hardware plugin configuration double parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationDoubleParameter : public CHardwarePluginConfigurationSimpleParameter<double>
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] defaultValue  Parameter default value
   //--------------------------------------------------------------
   CHardwarePluginConfigurationDoubleParameter(const std::string& name, const std::string& description, double defaultValue);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   ~CHardwarePluginConfigurationDoubleParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   // [END] CHardwarePluginConfigurationParameter implementation
};
