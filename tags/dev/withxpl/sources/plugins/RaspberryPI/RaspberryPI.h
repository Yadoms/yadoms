#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/event/EventHandler.hpp>

//--------------------------------------------------------------
/// \brief	This class is the RaspberryPI plugin
/// \note   This plugin send periodically temperature of the Raspberry
//--------------------------------------------------------------
class CRaspberryPI : public shared::event::CEventHandler, public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CRaspberryPI();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRaspberryPI();

   // IPlugin implementation
   virtual void doWork(int instanceUniqueId, const std::string& configuration, boost::asio::io_service * pluginIOService);
   virtual void updateConfiguration(const std::string& configuration);
  // [END] IPlugin implementation
};



