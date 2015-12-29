#pragma once

#include "IMegatecUpsConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CMegatecUpsConfiguration : public IMegatecUpsConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMegatecUpsConfiguration();

   // IMegatecUpsConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSerialPort() const;
   virtual bool upsBeepEnable() const;
   virtual double outuputShutdownDelay() const;
   virtual double outuputRestoreDelay() const;
   // [END] IMegatecUpsConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

