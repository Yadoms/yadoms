#pragma once
#include "IConfiguration.h"
#include "IEngine.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	OneWire engine factory
//--------------------------------------------------------------
class CFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CFactory();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CFactory();

   //--------------------------------------------------------------
   /// \brief Create appropriate engine (depending on OS and configuration)
   /// \param[in]	context yApi context
   /// \param[in] configuration  The 1-wire configuration
   //--------------------------------------------------------------
   static boost::shared_ptr<IEngine> createEngine(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const IConfiguration> configuration);
};