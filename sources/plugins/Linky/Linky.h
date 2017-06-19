#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "LinkyConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IDecoder.h"

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
                              const shared::CDataContainer& newConfigurationData);

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
                            const boost::shared_ptr<std::map<std::string, std::string>>& messages);

   //--------------------------------------------------------------
   /// \brief	                     Called when the Linky Receiver becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processLinkyConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	                     Called when the Linky Receiver becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processLinkyUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the Linky USB Reader
   //--------------------------------------------------------------
   void initLinkyReceiver() const;

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYPluginApi> context);
private:

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CLinkyConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The Linky protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<IDecoder> m_decoder;

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IReceiveBufferHandler> m_receiveBufferHandler;

   //--------------------------------------------------------------
   /// \brief  developerMode state
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

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
      kRunning
   };

   //--------------------------------------------------------------
   /// \brief	The plugin state
   //--------------------------------------------------------------
   ELinkyPluginState m_runningState;
};

