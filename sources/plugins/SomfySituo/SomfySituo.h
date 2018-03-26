#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "SomfySituoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>
#include <shared/event/EventTimer.h>
#include <shared/communication/AsyncPortConnectionNotification.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

DECLARE_ENUM_HEADER(EChannel,
   ((Channel1)(1))
   ((Channel2)(2))
   ((Channel3)(3))
   ((Channel4)(4))
   ((Channel5)(5))
);

//--------------------------------------------------------------
/// \brief	This plugin supports adapter for Somfy IO Situo Remote Controls
//--------------------------------------------------------------
class CSomfySituo : public plugin_cpp_api::IPlugin
{
public:
   typedef enum
   {
      Debug = 'D',
   } ConfigSituo;

   CSomfySituo();
   virtual ~CSomfySituo();

   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;

protected:

   void manageEvents(boost::shared_ptr<yApi::IYPluginApi> api);

   void send(const std::string& message,
             bool needAnswer = false);

   void send(const shared::communication::CByteBuffer& buffer,
             bool needAnswer = false);

   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device, const std::string& command);

   void processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   void processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification
                                    =
                                    boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& message);

   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   void destroyConnection();

   void protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api);

   static bool connectionsAreEqual(const CSomfySituoConfiguration& conf1,
                                   const CSomfySituoConfiguration& conf2);

   void processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> api,
      const boost::tokenizer<boost::char_separator<char>>& tokens) const;

   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
      const std::string& model, const std::string& version) const;

   //--------------------------------------------------------------
   /// \brief Send command to the remote control adapter
   //--------------------------------------------------------------
   void sendConfigCmd(ConfigSituo conf, int value);
   void sendGetStatusCmd();
   void sendChannelCmd();
   void sendUpCmd();
   void sendQuickUpCmd(int chan);
   void sendDownCmd();
   void sendQuickDownCmd(int chan);
   void sendMyCmd();
   void sendQuickMyCmd(int chan);
   void sendProgCmd();
   void sendQuickProgCmd(int chan);


private:
   CSomfySituoConfiguration m_configuration;
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;
   boost::shared_ptr<shared::communication::IBufferLogger> m_logger;
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   unsigned int m_protocolErrorCounter;
   shared::communication::CByteBuffer m_lastSentBuffer;
   static const std::string DeviceName;

   int m_activeChannel;
   bool m_channelSleep;
   boost::shared_ptr<yApi::historization::CCurtain> m_curtain;
   boost::shared_ptr<CSomfySituoReceiveBufferHandler> m_ReceiveBufferHandler;

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   static const std::map<std::string, int> m_somfyModels;
};
