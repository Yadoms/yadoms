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
   const size_t CIpcAdapter::m_maxMessageSize(100000);

   CIpcAdapter::CIpcAdapter(boost::shared_ptr<CYPluginApiImplementation> yPluginApi)
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
               auto messageWasReceived = m_receiveMessageQueue.timed_receive(message.get(),
                                                                             m_receiveMessageQueue.get_max_msg_size(),
                                                                             messageSize,
                                                                             messagePriority,
                                                                             boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(1));
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
      {
         YADOMS_LOG(error) << "CIpcAdapter::send : message is not fully initialized ==> ignored)";
         return;
      }

      if (pbMsg.ByteSize() > static_cast<int>(m_sendMessageQueue.get_max_msg_size()))
      {
         YADOMS_LOG(error) << "CIpcAdapter::send : message is too big (" << pbMsg.ByteSize() << " bytes) ==> ignored)";
         return;
      }

      boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);
      if (!pbMsg.SerializeToArray(m_sendBuffer.get(), pbMsg.GetCachedSize()))
      {
         YADOMS_LOG(error) << "CIpcAdapter::send : fail to serialize message (too big ?) ==> ignored)";
         return;
      }

      YADOMS_LOG(trace) << "[SEND] message " << pbMsg.OneOf_case() << " to plugin instance #" << m_pluginApi->getPluginId();

      m_sendMessageQueue.send(m_sendBuffer.get(), pbMsg.GetCachedSize(), 0);
   }

   void CIpcAdapter::send(const toPlugin::msg& pbMsg,
                          boost::function1<bool, const toYadoms::msg&> checkExpectedMessageFunction,
                          boost::function1<void, const toYadoms::msg&> onReceiveFunction,
                          const boost::posix_time::time_duration& timeout)
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

      if (receivedEvtHandler.waitForEvents(timeout) == shared::event::kTimeout)
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
         throw shared::exception::CInvalidParameter("message : fail to parse received data into protobuf format");

      YADOMS_LOG(trace) << "[RECEIVE] message " << toYadomsProtoBuffer.OneOf_case() << " from plugin instance #" << m_pluginApi->getPluginId() << (m_onReceiveHook ? " (onReceiveHook ENABLED)" : "");

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
      case toYadoms::msg::kUpdateDeviceDetails: processUpdateDeviceDetails(toYadomsProtoBuffer.updatedevicedetails());
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
      case toYadoms::msg::kYadomsInformationRequest: processYadomsInformationRequest(toYadomsProtoBuffer.yadomsinformationrequest());
         break;
      case toYadoms::msg::kRemoveDevice: processRemoveDeviceRequest(toYadomsProtoBuffer.removedevice());
         break;
      case toYadoms::msg::kAllDevicesRequest: processAllDevicesRequest(toYadomsProtoBuffer.alldevicesrequest());
         break;
      case toYadoms::msg::kRemoveKeyword: processRemoveKeywordRequest(toYadomsProtoBuffer.removekeyword());
         break;
      case toYadoms::msg::kAllKeywordsRequest: processAllKeywordsRequest(toYadomsProtoBuffer.allkeywordsrequest());
         break;
      case toYadoms::msg::kDeclareKeywords: processDeclareKeywords(toYadomsProtoBuffer.declarekeywords());
         break;
      case toYadoms::msg::kDeviceModelRequest: processDeviceModelRequest(toYadomsProtoBuffer.devicemodelrequest());
         break;
      case toYadoms::msg::kUpdateDeviceModel: processUpdateDeviceModel(toYadomsProtoBuffer.updatedevicemodel());
         break;
      case toYadoms::msg::kDeviceConfigurationRequest: processDeviceConfigurationRequest(toYadomsProtoBuffer.deviceconfigurationrequest());
         break;
      case toYadoms::msg::kUpdateDeviceConfiguration: processUpdateDeviceConfiguration(toYadomsProtoBuffer.updatedeviceconfiguration());
         break;
      default:
         throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % toYadomsProtoBuffer.OneOf_case()).str());
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

      shared::CDataContainer dc(msg.custommessagedata());

      auto values = dc.get<std::map<std::string, std::string>>();

      m_pluginApi->setPluginState(state, msg.custommessageid(), values);
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

   void CIpcAdapter::processUpdateDeviceDetails(const toYadoms::UpdateDeviceDetails& msg) const
   {
      m_pluginApi->updateDeviceDetails(msg.device(),
                                       shared::CDataContainer(msg.details()));
   }

   void CIpcAdapter::processAllDevicesRequest(const toYadoms::AllDevicesRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_alldevicesanswer();
      auto devices = m_pluginApi->getAllDevices();
      std::copy(devices.begin(), devices.end(), RepeatedFieldBackInserter(answer->mutable_devices()));
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
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords;
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
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> dataVect;
      for (auto value = msg.value().begin(); value != msg.value().end(); ++value)
      {
         dataVect.push_back(boost::make_shared<CFromPluginHistorizer>(value->historizable(), value->formattedvalue()));
      }
      m_pluginApi->historizeData(msg.device(), dataVect);
   }

   void CIpcAdapter::processYadomsInformationRequest(const toYadoms::YadomsInformationRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_yadomsinformationanswer();
      auto yadomsInformation = m_pluginApi->getYadomsInformation();
      answer->set_developpermode(yadomsInformation->developperMode());
      answer->set_version(yadomsInformation->version());

      switch (yadomsInformation->releaseType())
      {
      case shared::versioning::EReleaseType::kStableValue: answer->set_releasetype(toPlugin::YadomsInformationAnswer_EReleaseType_Stable);
         break;
      case shared::versioning::EReleaseType::kReleaseCandidateValue: answer->set_releasetype(toPlugin::YadomsInformationAnswer_EReleaseType_ReleaseCandidate);
         break;
      case shared::versioning::EReleaseType::kBetaValue: answer->set_releasetype(toPlugin::YadomsInformationAnswer_EReleaseType_Beta);
         break;
      default:
         answer->set_releasetype(toPlugin::YadomsInformationAnswer_EReleaseType_Beta);
         break;
      }

      auto location = answer->mutable_location();
      location->set_longitude(yadomsInformation->location()->longitude());
      location->set_latitude(yadomsInformation->location()->latitude());
      location->set_altitude(yadomsInformation->location()->altitude());
      send(ans);
   }

   void CIpcAdapter::processRemoveDeviceRequest(const toYadoms::RemoveDevice& msg) const
   {
      m_pluginApi->removeDevice(msg.device());
   }

   void CIpcAdapter::processRemoveKeywordRequest(const toYadoms::RemoveKeyword& msg) const
   {
      m_pluginApi->removeKeyword(msg.device(),
                                 msg.keyword());
   }

   void CIpcAdapter::processAllKeywordsRequest(const toYadoms::AllKeywordsRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_allkeywordsanswer();
      auto keywords = m_pluginApi->getAllKeywords(msg.device());
      std::copy(keywords.begin(), keywords.end(), RepeatedFieldBackInserter(answer->mutable_keywords()));
      send(ans);
   }

   void CIpcAdapter::processDeclareKeywords(const toYadoms::DeclareKeywords& msg) const
   {
      std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> keywords;
      for (auto keyword = msg.keywords().begin(); keyword != msg.keywords().end(); ++keyword)
         keywords.push_back(boost::make_shared<CFromPluginHistorizer>(*keyword));

      m_pluginApi->declareKeywords(msg.device(),
                                   keywords);
   }

   void CIpcAdapter::processDeviceModelRequest(const toYadoms::DeviceModelRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_devicemodelanswer();
      answer->set_model(m_pluginApi->getDeviceModel(msg.device()));
      send(ans);
   }

   void CIpcAdapter::processUpdateDeviceModel(const toYadoms::UpdateDeviceModel& msg) const
   {
      m_pluginApi->updateDeviceModel(msg.device(),
                                     msg.model());
   }

   void CIpcAdapter::processDeviceConfigurationRequest(const toYadoms::DeviceConfigurationRequest& msg)
   {
      toPlugin::msg ans;
      auto answer = ans.mutable_deviceconfigurationanswer();
      answer->set_configuration(m_pluginApi->getDeviceConfiguration(msg.device()).serialize());
      send(ans);
   }

   void CIpcAdapter::processUpdateDeviceConfiguration(const toYadoms::UpdateDeviceConfiguration& msg) const
   {
      m_pluginApi->updateDeviceConfiguration(msg.device(),
                                             shared::CDataContainer(msg.configuration()));
   }

   void CIpcAdapter::postStopRequest()
   {
      toPlugin::msg msg;
      auto message = msg.mutable_system();
      message->set_type(toPlugin::System_EventType_kRequestStop);

      send(msg);
   }

   void CIpcAdapter::postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information, const boost::filesystem::path& dataPath, const boost::filesystem::path& logFile)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_init();
      serializers::CInformation(information).toPb(message->mutable_plugininformation());
      message->set_datapath(dataPath.string());
      message->set_logfile(logFile.string());

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

      auto success = false;
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

   void CIpcAdapter::postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request)
   {
      toPlugin::msg req;
      auto message = req.mutable_deviceconfigurationschemarequest();
      message->set_device(request->device());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const toYadoms::msg& ans) -> bool
              {
                 return ans.has_deviceconfigurationschemaanswer();
              },
              [&](const toYadoms::msg& ans) -> void
              {
                 success = ans.deviceconfigurationschemaanswer().success();
                 result = ans.deviceconfigurationschemaanswer().result();
              });
      }
      catch (std::exception& e)
      {
         request->sendError((boost::format("Plugin doesn't answer to device configuration schema request : %1%") % e.what()).str());
      }

      if (success)
         request->sendSuccess(shared::CDataContainer(result));
      else
         request->sendError(result);
   }

   void CIpcAdapter::postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration>& command)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_setdeviceconfiguration();
      message->set_device(command->device());
      message->set_configuration(command->configuration().serialize());
      send(msg);
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

   void CIpcAdapter::postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery)
   {
      toPlugin::msg req;
      auto message = req.mutable_extraquery();
      message->set_query(extraQuery->getData().query());
      message->set_data(extraQuery->getData().data().serialize());

      auto success = false;
      std::string result;

      try
      {
         send(req,
              [&](const toYadoms::msg& ans) -> bool
              {
                 return ans.has_extraqueryanswer();
              },
              [&](const toYadoms::msg& ans) -> void
              {
                 success = ans.extraqueryanswer().success();
                 result = ans.extraqueryanswer().result();
              });
      }
      catch (std::exception& e)
      {
         extraQuery->sendError((boost::format("Plugin doesn't answer to extra query : %1%") % e.what()).str());
      }

      if (success)
         extraQuery->sendSuccess(shared::CDataContainer(result));
      else
         extraQuery->sendError(result);
   }

   void CIpcAdapter::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request)
   {
      toPlugin::msg req;
      auto message = req.mutable_manuallydevicecreation();
      message->set_name(request->getData().getDeviceName());
      message->set_configuration(request->getData().getConfiguration().serialize());

      auto success = false;
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

   void CIpcAdapter::postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event)
   {
      toPlugin::msg msg;
      auto message = msg.mutable_deviceremoved();
      message->set_device(event->device());
      message->set_details(event->details().serialize());
      send(msg);
   }
} // namespace pluginSystem


