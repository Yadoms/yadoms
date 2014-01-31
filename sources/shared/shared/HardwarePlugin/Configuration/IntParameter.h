#pragma once

#include <shared/Export.h>
#include "SimpleParameter.hpp"

//--------------------------------------------------------------
/// \type Hardware plugin configuration int parameter
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginConfigurationIntParameter : public CHardwarePluginConfigurationSimpleParameter<int>
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] name  Parameter name
   /// \param[in] description  Parameter description
   /// \param[in] defaultValue  Parameter default value
   //--------------------------------------------------------------
   CHardwarePluginConfigurationIntParameter(const std::string& name, const std::string& description, int defaultValue);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginConfigurationIntParameter() {}

   // CHardwarePluginConfigurationParameter implementation
   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const;
   // [END] CHardwarePluginConfigurationParameter implementation
};
