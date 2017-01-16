#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "ZiBlueConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "ZiBlueTransceiver.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the ZWave plugin entry point
//--------------------------------------------------------------
class CZiBlue : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CZiBlue();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CZiBlue();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CZiBlueConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The ziblue transceiver
   //--------------------------------------------------------------
   boost::shared_ptr<CZiBlueTransceiver> m_transceiver;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the ZiBlue RFP1000
   /// \param [in] eventHandler     Event handler to be notified on events on the connection
   //--------------------------------------------------------------
   void createConnection(shared::event::CEventHandler& eventHandler);
   void destroyConnection();
   void processZiBlueConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);
   void errorProcess(boost::shared_ptr<yApi::IYPluginApi> api);
   void processZiBlueUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);
   void processZiBlueBinaryDataReceived(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CByteBuffer& data);
   void processZiBlueCommandAnswerReceived(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CStringBuffer& data);
   void initZiBlue(boost::shared_ptr<yApi::IYPluginApi> api);
   void send(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<std::queue<shared::communication::CStringBuffer>> buffers);
   void send(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CStringBuffer& buffer, bool needAnswer);
};



