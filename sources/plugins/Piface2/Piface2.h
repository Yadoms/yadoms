#pragma once
#include <plugin_cpp_api/IPlugin.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Event IDs
enum
{
   kEvtIOStateReceived = yApi::IYPluginApi::kPluginFirstEventId // Always start from shared::event::CEventHandler::kUserFirstId
};

//--------------------------------------------------------------
/// \brief	This class is the Piface2 plugin
/// \note   This plugin configure GPIOs of the Pi board for using the Piface2 extension board.
//--------------------------------------------------------------
class CPiface2 : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPiface2();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPiface2();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_deviceName;
};

