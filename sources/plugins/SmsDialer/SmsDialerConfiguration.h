#pragma once

#include "ISmsDialerConfiguration.h"
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSmsDialerConfiguration : public ISmsDialerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmsDialerConfiguration();

   // ISmsDialerConfiguration implementation
   void initializeWith(const boost::shared_ptr<shared::CDataContainer> &data) override;
   std::string getGammuPort() const override;
   std::string getGammuProtocol() const override;
   std::string getPhonePIN() const override;
   bool hasPINCode() const override;
   // [END] ISmsDialerConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

