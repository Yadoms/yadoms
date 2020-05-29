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
   virtual void initializeWith(const boost::shared_ptr<shared::CDataContainer> &data);
   virtual std::string getGammuPort() const;
   virtual std::string getGammuProtocol() const;
   virtual std::string getPhonePIN() const;
   virtual bool hasPINCode() const;
   // [END] ISmsDialerConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_data;
};

