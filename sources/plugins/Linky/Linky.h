#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "LinkyConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IDecoder.h"
#include "IGPIOManager.h"
#include "LinkyReceiveBufferHandler.h"
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "LinkyFactory.h"
#include "ProtocolManager.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the Linky plugin
/// \note   This plugin receive information from a specific USB module that read some security devices from Linky
//--------------------------------------------------------------
class CLinky : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CLinky();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CLinky();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> context) override;
   // [END] IPlugin implementation
protected:

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the Linky USB Reader
   /// \param [in] context           Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                        Update the configuration of the plugin
   /// \param [in] context              Plugin execution context (Yadoms API)
   /// \param [in] newConfigurationData The new configuration
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const boost::shared_ptr<shared::CDataContainer>& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Destroy the serial connection
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Called when received a message from the Linky Receiver
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] messages         The buffer with the received information
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages);

   //--------------------------------------------------------------
   /// \brief	                     Called when the Linky Receiver becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processLinkyConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	                     Called when the Linky Receiver becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] notification     The connection notification data
   //--------------------------------------------------------------
   void processLinkyUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification = boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the Linky USB Reader
   //--------------------------------------------------------------
   void initLinkyReceiver() const;

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYPluginApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Change protocol (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void changeProtocol(boost::shared_ptr<yApi::IYPluginApi> api);
private:

   //-----------------------------------------------------
   ///\brief The plugin state
   //-----------------------------------------------------
   enum ELinkyPluginState
   {
      kUndefined = 0,
      kStop,
      kConnecting,
      kConnectionLost,
      kErDFCounterdesactivated,
      kupdateConfiguration,
      kRunning,
      kError
   };

   void setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, ELinkyPluginState newState);
   void setPluginErrorState(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& ErrorMessageI18n,
                            const std::map<std::string, std::string>& ErrorMessageI18nParameters);

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	Periodic sampling event
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_periodicSamplingTimer;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<CLinkyConfiguration> m_configuration;

   //--------------------------------------------------------------
   /// \brief	The Linky protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<IDecoder> m_decoder[2];

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<CLinkyReceiveBufferHandler> m_receiveBufferHandler;

   //--------------------------------------------------------------
   /// \brief  developerMode state
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief	The plugin state
   //--------------------------------------------------------------
   ELinkyPluginState m_runningState;

   //--------------------------------------------------------------
   /// \brief	The manager of the protocol for each line
   //--------------------------------------------------------------
   boost::shared_ptr<CProtocolManager> m_protocolManager[2];

   //--------------------------------------------------------------
   /// \brief	    List of port to read
   //--------------------------------------------------------------
   boost::shared_ptr<IGPIOManager> m_GPIOManager;
};