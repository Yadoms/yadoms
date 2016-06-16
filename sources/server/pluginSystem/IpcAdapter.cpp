#include "stdafx.h"
#include "IpcAdapter.h"
#include <shared/communication/MessageQueueRemover.hpp>
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "serializers/Information.h"
#include "FromPluginHistorizer.h"

namespace pluginSystem
{
   const size_t CIpcAdapter::m_maxMessages(100);
   const size_t CIpcAdapter::m_maxMessageSize(10000);

   CIpcAdapter::CIpcAdapter(boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> yPluginApi)
      : m_pluginApi(yPluginApi),
        m_id(createId()),
        m_sendMessageQueueId(m_id + ".toPlugin"),
        m_receiveMessageQueueId(m_id + ".toYadoms"),
        m_sendMessageQueueRemover(m_sendMessageQueueId),
        m_receiveMessageQueueRemover(m_receiveMessageQueueId),
        m_sendMessageQueue(boost::interprocess::create_only, m_sendMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
        m_receiveMessageQueue(boost::interprocess::create_only, m_receiveMessageQueueId.c_str(), m_maxMessages, m_maxMessageSize),
        m_sendBuffer(boost::make_shared<unsigned char[]>(m_sendMessageQueue.get_max_msg_size())),
        m_messageQueueReceiveThread(boost::thread(&CIpcAdapter::messageQueueReceiveThreaded, this))
   {
   }

   CIpcAdapter::~CIpcAdapter()
   {
      m_messageQueueReceiveThread.interrupt();
      m_messageQueueReceiveThread.join();
   }

   std::string CIpcAdapter::id() const
   {
      return m_id;
   }

   std::string CIpcAdapter::createId()
   {
      std::stringstream ss;
      ss << "yPlugin." << boost::uuids::random_generator()();
      return ss.str();
   }

   void CIpcAdapter::messageQueueReceiveThreaded()
   {
      // Verify that the version of the library that we linked against is
      // compatible with the version of the headers we compiled against.
      GOOGLE_PROTOBUF_VERIFY_VERSION;

      YADOMS_LOG(information) << "Message queue ID : " << m_id;

      try
      {
         auto message(boost::make_shared<unsigned char[]>(m_receiveMessageQueue.get_max_msg_size()));
         size_t messageSize;
         unsigned int messagePriority;
         while (true)
         {
            try
            {
               // boost::interprocess::message_queue::receive is not responding to boost thread interruption, so we need to do some
               // polling and call boost::this_thread::interruption_point to exit properly
               // Note that boost::interprocess::message_queue::timed_receive requires universal time to work (can not use shared::currentTime::Provider)
               auto messageWasReceived = m_receiveMessageQueue.timed_receive(message.get(), m_receiveMessageQueue.get_max_msg_size(), messageSize, messagePriority,
                                                                             boost::posix_time::ptime(boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1)));
               boost::this_thread::interruption_point();

               if (messageWasReceived)
                  processMessage(message, messageSize);
            }
            catch (std::exception& ex)
            {
               YADOMS_LOG(error) << "Error receiving/processing queue message : " << ex.what();
            }
         }
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         YADOMS_LOG(error) << "Error creating/using message queues (" << m_id << ") in context accessor : " << ex.what();
      }
      catch (boost::thread_interrupted&)
      {
      }
      catch (std::exception& ex)
      {
         YADOMS_LOG(error) << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Unknown error";
      }

      YADOMS_LOG(debug) << "Close message queues";

      // Delete all global objects allocated by libprotobuf.
      google::protobuf::ShutdownProtobufLibrary();
   }

   void CIpcAdapter::send(const toPlugin::msg& pbMsg)
   {
      if (!pbMsg.IsInitialized())
         throw std::overflow_error("CIpcAdapter::send : message is not fully initialized");

      if (pbMsg.ByteSize() > static_cast<int>(m_sendMessageQueue.get_max_msg_size()))
         throw std::overflow_error((boost::format("CIpcAdapter::send : message is too big (%1% bytes)") % pbMsg.ByteSize()).str());

      if (!pbMsg.SerializeToArray(m_sendBuffer.get(), m_sendMessageQueue.get_max_msg_size()))
         throw std::overflow_error("CIpcAdapter::send : fail to serialize message (too big ?)");

      m_sendMessageQueue.send(m_sendBuffer.get(), pbMsg.GetCachedSize(), 0);
   }

   void CIpcAdapter::send(const toPlugin::msg& pbMsg,
                          boost::function1<bool, const toYadoms::msg&> checkExpectedMessageFunction,
                          boost::function1<void, const toYadoms::msg&> onReceiveFunction)
   {
      shared::event::CEventHandler receivedEvtHandler;

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook = [&](const toYadoms::msg& receivedMsg)-> bool
            {
               if (!checkExpectedMessageFunction(receivedMsg))
                  return false;

               receivedEvtHandler.postEvent<const toYadoms::msg>(shared::event::kUserFirstId, receivedMsg);
               return true;
            };
      }

      send(pbMsg);

      if (receivedEvtHandler.waitForEvents(boost::posix_time::minutes(1)) == shared::event::kTimeout)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook.clear();
         throw std::runtime_error((boost::format("No answer from plugin when sending message %1%") % pbMsg.OneOf_case()).str());
      }

      onReceiveFunction(receivedEvtHandler.getEventData<const toYadoms::msg>());
   }

   void CIpcAdapter::processMessage(boost::shared_ptr<const unsigned char[]> message, size_t messageSize)
   {
      if (messageSize < 1)
         throw shared::exception::CInvalidParameter("messageSize");

      // Unserialize message
      toYadoms::msg toYadomsProtoBuffer;
      if (!toYadomsProtoBuffer.ParseFromArray(message.get(), messageSize))
         throw shared::exception::CInvalidParameter("message");

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         if (m_onReceiveHook && m_onReceiveHook(toYadomsProtoBuffer))
         {
            m_onReceiveHook.clear();
            return;
         }
      }

      // Process message
      switch (toYadomsProtoBuffer.OneOf_case())
      {
      case toYadoms::msg::kPluginState: processSetPluginState(toYadomsProtoBuffer.pluginstate());
         break;
      case toYadoms::msg::kConfigurationRequest: processGetConfiguration(toYadomsProtoBuffer.configurationrequest());
         break;
      case toYadoms::msg::kDeviceExists: processDeviceExistsRequest(toYadomsProtoBuffer.deviceexists());
         break;
      case toYadoms::msg::kDeviceDetails: processDeviceDetailsRequest(toYadomsProtoBuffer.devicedetails());
         break;
      case toYadoms::msg::kKeywordExists: processKeywordExistsRequest(toYadomsProtoBuffer.keywordexists());
         break;
      case toYadoms::msg::kDeclareDevice: processDeclareDevice(toYadomsProtoBuffer.declaredevice());
         break;
      case toYadoms::msg::kDeclareKeyword: processDeclareKeyword(toYadomsProtoBuffer.declarekeyword());
         break;
      case toYadoms::msg::kRecipientValueRequest: processRecipientValueRequest(toYadomsProtoBuffer.recipientvaluerequest());
         break;
      case toYadoms::msg::kFindRecipientsFromFieldRequest: processFindRecipientsFromFieldRequest(toYadomsProtoBuffer.findrecipientsfromfieldrequest());
         break;
      case toYadoms::msg::kRecipientFieldExitsRequest: processRecipientFieldExitsRequest(toYadomsProtoBuffer.recipientfieldexitsrequest());
         break;
      case toYadoms::msg::kHistorizeData: processHistorizeData(toYadomsProtoBuffer.historizedata());
         break;
      default:
         throw shared::exception::CInvalidParameter((boost::format("message : %1%") % toYadomsProtoBuffer.OneOf_case()).str());
      }
   }

   void CIpcAdapter::processSetPluginState(const toYadoms::SetPluginState& msg) const
   {
      shared::plugin::yPluginApi::historization::EPluginState state;
      switch (msg.pluginstate())
      {
      case toYadoms::SetPluginState_EPluginState_kUnknown: state = shared::plugin::yPluginApi::historization::EPluginState::kUnknownValue;
         break;
      case toYadoms::SetPluginState_EPluginState_kError: state = shared::plugin::yPluginApi::historization::EPluginState::kErrorValue;
         break;
      case toYadoms::SetPluginState_EPluginState_kStopped: state = shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue;
         break;
      case toYadoms::SetPluginState_EPluginState_kRunning: state = shared::plugin::yPluginApi::historization::EPluginState::kRunningValue;
         break;
      case toYadoms::SetPluginState_EPluginState_kCustom: state = shared::plugin::yPluginApi::historization::EPluginState::kCustomValue;
         break;
      default:
         throw std::out_of_range((boost::format("Unsupported plugin state received : %1%") % msg.pluginstate()).str());
      }

      m_pluginApi->setPluginState(state, msg.custommessageid(), msg.custommessagedata());
   }

   void CIpcAdapter::processGetConfiguration(const toYadoms::ConfigurationRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_configurationanswer();
      answer->set_configuration(m_pluginApi->getConfiguration().serialize());
      send(ans);
   }

   void CIpcAdapter::processDeviceExistsRequest(const toYadoms::DeviceExitsRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_deviceexists();
      answer->set_exists(m_pluginApi->deviceExists(msg.device()));
      send(ans);
   }

   void CIpcAdapter::processDeviceDetailsRequest(const toYadoms::DeviceDetailsRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_devicedetails();
      answer->set_details(m_pluginApi->getDeviceDetails(msg.device()).serialize());
      send(ans);
   }

   void CIpcAdapter::processKeywordExistsRequest(const toYadoms::KeywordExitsRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_keywordexists();
      answer->set_exists(m_pluginApi->keywordExists(msg.device(), msg.keyword()));
      send(ans);
   }

   void CIpcAdapter::processDeclareDevice(const toYadoms::DeclareDevice& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > keywords;
      for (auto keyword = msg.keywords().begin(); keyword != msg.keywords().end(); ++keyword)
         keywords.push_back(boost::make_shared<CFromPluginHistorizer>(*keyword));

      m_pluginApi->declareDevice(msg.device(),
                                 msg.model(),
                                 keywords,
                                 msg.has_details() ? shared::CDataContainer(msg.details()) : shared::CDataContainer::EmptyContainer);
   }

   void CIpcAdapter::processDeclareKeyword(const toYadoms::DeclareKeyword& msg) const
   {
      m_pluginApi->declareKeyword(msg.device(),
                                  boost::make_shared<CFromPluginHistorizer>(msg.keyword()),
                                  msg.has_details() ? shared::CDataContainer(msg.details()) : shared::CDataContainer::EmptyContainer);
   }

   void CIpcAdapter::processRecipientValueRequest(const toYadoms::RecipientValueRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_recipientvalue();
      answer->set_value(m_pluginApi->getRecipientValue(msg.recipientid(), msg.fieldname()));
      send(ans);
   }

   void CIpcAdapter::processFindRecipientsFromFieldRequest(const toYadoms::FindRecipientsFromFieldRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_findrecipientsfromfieldanswer();
      auto recipientIds = m_pluginApi->findRecipientsFromField(msg.fieldname(), msg.expectedfieldvalue());
      std::copy(recipientIds.begin(), recipientIds.end(), RepeatedFieldBackInserter(answer->mutable_recipientids()));
      send(ans);
   }

   void CIpcAdapter::processRecipientFieldExitsRequest(const toYadoms::RecipientFieldExitsRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_recipientfieldexitsanswer();
      answer->set_exists(m_pluginApi->recipientFieldExists(msg.fieldname()));
      send(ans);
   }

   void CIpcAdapter::processHistorizeData(const toYadoms::HistorizeData& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> > dataVect;
      for (auto value = msg.value().begin(); value != msg.value().end(); ++value)
      {
         dataVect.push_back(boost::make_shared<CFromPluginHistorizer>(value->historizable(), value->formattedvalue()));
      }
      m_pluginApi->historizeData(msg.device(), dataVect);
   }


   void CIpcAdapter::postStopRequest()
   {
      toPlugin::msg msg;
      auto message = msg.mutable_system();
      message->set_type(toPlugin::System_EventType_kRequestStop);

      send(msg);
   }

   void CIpcAdapter::postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information)
   {
      toPlugin::msg msg;
      serializers::CInformation(information).toPb(msg.mutable_plugininformation());

      send(msg);
   }

   void CIpcAdapter::postUpdateConfiguration(const shared::CDataContainer& newConfiguration)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_updateconfiguration();
      message->set_configuration(newConfiguration.serialize());
      send(msg);
   }

   void CIpcAdapter::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request)
   {
      toPlugin::msg req;
      auto message = req.mutable_bindingquery();
      message->set_query(request->getData().getQuery());

      bool success;
      std::string result;

      try
      {
         send(req,
              [&](const toYadoms::msg& ans) -> bool
              {
                 return ans.has_bindingqueryanswer();
              },
              [&](const toYadoms::msg& ans) -> void
              {
                 success = ans.bindingqueryanswer().success();
                 result = ans.bindingqueryanswer().result();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to binding query : %1%") % e.what()).str());
      }

      if (success)
         request->sendSuccess(shared::CDataContainer(result));
      else
         request->sendError(result);
   }

   void CIpcAdapter::postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_devicecommand();
      message->set_device(deviceCommand->getDevice());
      message->set_keyword(deviceCommand->getKeyword());
      message->set_body(deviceCommand->getBody());
      send(msg);
   }

   void CIpcAdapter::postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_extracommand();
      message->set_command(extraCommand->getCommand());
      message->set_data(extraCommand->getData().serialize());
      send(msg);
   }

   void CIpcAdapter::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request)
   {
      toPlugin::msg req;
      auto message = req.mutable_manuallydevicecreation();
      message->set_name(request->getData().getDeviceName());
      message->set_configuration(request->getData().getConfiguration().serialize());

      bool success;
      std::string result;

      try
      {
         send(req,
              [&](const toYadoms::msg& ans) -> bool
              {
                 return ans.has_manuallydevicecreationanswer();
              },
              [&](const toYadoms::msg& ans) -> void
              {
                 success = ans.manuallydevicecreationanswer().has_sucess();
                 result = success ?
                             ans.manuallydevicecreationanswer().sucess().newdevicename() :
                             ans.manuallydevicecreationanswer().error().message();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to binding query : %1%") % e.what()).str());
      }

      if (success)
         request->sendSuccess(result);
      else
         request->sendError(result);
   }

} // namespace pluginSystem

