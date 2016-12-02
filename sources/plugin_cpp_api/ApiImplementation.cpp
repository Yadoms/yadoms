#include "stdafx.h"
#include "ApiImplementation.h"
#include <shared/DataContainer.h>
#include "PluginInformation.h"
#include "BindingQuery.h"
#include "DeviceCommand.h"
#include "ExtraQuery.h"
#include "ManuallyDeviceCreation.h"
#include "DeviceConfigurationSchemaRequest.h"
#include "SetDeviceConfiguration.h"
#include "DeviceRemoved.h"
#include "YadomsInformation.h"

namespace plugin_cpp_api
{
   CApiImplementation::CApiImplementation()
      : m_initialized(false),
        m_stopRequested(false)
   {
   }

   CApiImplementation::~CApiImplementation()
   {
   }

   void CApiImplementation::setSendingMessageQueue(boost::shared_ptr<boost::interprocess::message_queue> sendMessageQueue)
   {
      m_sendMessageQueue = sendMessageQueue;
      m_mqBuffer = boost::make_shared<unsigned char[]>(m_sendMessageQueue->get_max_msg_size());
   }

   bool CApiImplementation::stopRequested() const
   {
      return m_stopRequested;
   }

   void CApiImplementation::send(const toYadoms::msg& msg) const
   {
      try
      {
         if (!m_sendMessageQueue)
            throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\", plugin API not ready to send message") % msg.descriptor()->full_name()).str());

         if (!msg.IsInitialized())
            throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\", request is not fully initialized") % msg.descriptor()->full_name()).str());

         if (msg.ByteSize() > static_cast<int>(m_sendMessageQueue->get_max_msg_size()))
            throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\", request is too big") % msg.descriptor()->full_name()).str());

         boost::lock_guard<boost::recursive_mutex> lock(m_sendMutex);
         if (!msg.SerializeToArray(m_mqBuffer.get(), msg.GetCachedSize()))
            throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\", fail to serialize request (too big ?)") % msg.descriptor()->full_name()).str());

         m_sendMessageQueue->send(m_mqBuffer.get(), msg.GetCachedSize(), 0);
      }
      catch (boost::interprocess::interprocess_exception& ex)
      {
         throw std::runtime_error((boost::format("CApiImplementation::send \"%1%\" : error :%2%") % msg.descriptor()->full_name() % ex.what()).str());
      }
   }

   void CApiImplementation::send(const toYadoms::msg& msg,
                                 boost::function1<bool, const toPlugin::msg&> checkExpectedMessageFunction,
                                 boost::function1<void, const toPlugin::msg&> onReceiveFunction) const
   {
      shared::event::CEventHandler receivedEvtHandler;
      enum
         {
            kExpectedEventReceived = shared::event::kUserFirstId,
            kErrorReceived
         };

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         m_onReceiveHook = [&](const toPlugin::msg& receivedMsg)-> bool
            {
               if (receivedMsg.has_error())
               {
                  receivedEvtHandler.postEvent<const toPlugin::msg>(kErrorReceived, receivedMsg);
                  return true;
               }

               if (!checkExpectedMessageFunction(receivedMsg))
                  return false;

               receivedEvtHandler.postEvent<const toPlugin::msg>(kExpectedEventReceived, receivedMsg);
               return true;
            };
      }

      send(msg);

      switch (receivedEvtHandler.waitForEvents(boost::posix_time::minutes(1)))
      {
      case kExpectedEventReceived:
         {
            onReceiveFunction(receivedEvtHandler.getEventData<const toPlugin::msg>());
            break;
         }

      case kErrorReceived:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("Error \"%1%\" received from Yadoms when sending message %2%") % receivedEvtHandler.getEventData<const toPlugin::msg>().error() % msg.OneOf_case()).str());
         }

      case shared::event::kTimeout:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("No answer from Yadoms when sending message %1%") % msg.OneOf_case()).str());
         }

      default:
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
            m_onReceiveHook.clear();
            throw std::runtime_error((boost::format("Invalid event received %1% when sending message %2%") % receivedEvtHandler.getEventId() % msg.OneOf_case()).str());
         }
      }
   }

   void CApiImplementation::onReceive(boost::shared_ptr<const unsigned char[]> message,
                                      size_t messageSize)
   {
      if (messageSize < 1)
         throw std::runtime_error("CApiImplementation::onReceive : received Yadoms answer is zero length");

      toPlugin::msg toPluginProtoBuffer;
      if (!toPluginProtoBuffer.ParseFromArray(message.get(), messageSize))
         throw shared::exception::CInvalidParameter((boost::format("message : fail to parse received data into protobuf format (received buffer size is %1%)") % messageSize).str());

      if (!m_initialized)
      {
         if (toPluginProtoBuffer.has_init())
            processInit(toPluginProtoBuffer.init());
         else
            throw shared::exception::CInvalidParameter((boost::format("Unexpected message %1% when initialization") % toPluginProtoBuffer.OneOf_case()).str());
         return;
      }

      {
         boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
         if (m_onReceiveHook && m_onReceiveHook(toPluginProtoBuffer))
         {
            m_onReceiveHook.clear();
            return;
         }
      }

      switch (toPluginProtoBuffer.OneOf_case())
      {
      case toPlugin::msg::kSystem: processSystem(toPluginProtoBuffer.system());
         break;
      case toPlugin::msg::kUpdateConfiguration: processUpdateConfiguration(toPluginProtoBuffer.updateconfiguration());
         break;
      case toPlugin::msg::kBindingQuery: processBindingQuery(toPluginProtoBuffer.bindingquery());
         break;
      case toPlugin::msg::kDeviceCommand: processDeviceCommand(toPluginProtoBuffer.devicecommand());
         break;
      case toPlugin::msg::kExtraQuery: processExtraQuery(toPluginProtoBuffer.extraquery());
         break;
      case toPlugin::msg::kDeviceConfigurationSchemaRequest: processDeviceConfigurationSchemaRequest(toPluginProtoBuffer.deviceconfigurationschemarequest());
         break;
      case toPlugin::msg::kSetDeviceConfiguration: processSetDeviceConfiguration(toPluginProtoBuffer.setdeviceconfiguration());
         break;
      case toPlugin::msg::kManuallyDeviceCreation: processManuallyDeviceCreation(toPluginProtoBuffer.manuallydevicecreation());
         break;
      case toPlugin::msg::kDeviceRemoved: processDeviceRemoved(toPluginProtoBuffer.deviceremoved());
         break;
      default:
         throw shared::exception::CInvalidParameter((boost::format("message : unknown message type %1%") % toPluginProtoBuffer.OneOf_case()).str());
      }
   }


   void CApiImplementation::waitInitialized() const
   {
      std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
      if (!m_initialized)
         m_initializationCondition.wait(lock);
   }

   void CApiImplementation::processSystem(const toPlugin::System& msg)
   {
      if (msg.type() == toPlugin::System_EventType_kRequestStop)
      {
         // Request the main thread to stop
         m_stopRequested = true;
         getEventHandler().postEvent(kEventStopRequested);

         // Stop this thread
         throw boost::thread_interrupted();
      }
   }

   void CApiImplementation::processInit(const toPlugin::Init& msg)
   {
      m_pluginInformation = boost::make_shared<CPluginInformation>(boost::make_shared<const toPlugin::Information>(msg.plugininformation()));
      m_dataPath = boost::make_shared<const boost::filesystem::path>(msg.datapath());
      setInitialized();
   }

   void CApiImplementation::processUpdateConfiguration(const toPlugin::Configuration& msg)
   {
      m_pluginEventHandler.postEvent(kEventUpdateConfiguration, shared::CDataContainer(msg.configuration()));
   }

   void CApiImplementation::setInitialized()
   {
      if (!!m_pluginInformation && !!m_dataPath)
      {
         std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
         m_initialized = true;
         m_initializationCondition.notify_one();
      }
   }

   void CApiImplementation::processBindingQuery(const toPlugin::BindingQuery& msg)
   {
      boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> query = boost::make_shared<CBindingQuery>(msg,
                                                                                                                    [&](const shared::CDataContainer& r)
                                                                                                                    {
                                                                                                                       toYadoms::msg ans;
                                                                                                                       auto answer = ans.mutable_bindingqueryanswer();
                                                                                                                       answer->set_success(true);
                                                                                                                       answer->set_result(r.serialize());
                                                                                                                       send(ans);
                                                                                                                    },
                                                                                                                    [&](const std::string& r)
                                                                                                                    {
                                                                                                                       toYadoms::msg ans;
                                                                                                                       auto answer = ans.mutable_bindingqueryanswer();
                                                                                                                       answer->set_success(false);
                                                                                                                       answer->set_result(r);
                                                                                                                       send(ans);
                                                                                                                    });

      m_pluginEventHandler.postEvent(kBindingQuery, query);
   }

   void CApiImplementation::processDeviceConfigurationSchemaRequest(const toPlugin::DeviceConfigurationSchemaRequest& msg)
   {
      boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> query = boost::make_shared<CDeviceConfigurationSchemaRequest>(msg,
                                                                                                                                                     [&](const shared::CDataContainer& r)
                                                                                                                                                     {
                                                                                                                                                        toYadoms::msg ans;
                                                                                                                                                        auto answer = ans.mutable_deviceconfigurationschemaanswer();
                                                                                                                                                        answer->set_success(true);
                                                                                                                                                        answer->set_result(r.serialize());
                                                                                                                                                        send(ans);
                                                                                                                                                     },
                                                                                                                                                     [&](const std::string& r)
                                                                                                                                                     {
                                                                                                                                                        toYadoms::msg ans;
                                                                                                                                                        auto answer = ans.mutable_deviceconfigurationschemaanswer();
                                                                                                                                                        answer->set_success(false);
                                                                                                                                                        answer->set_result(r);
                                                                                                                                                        send(ans);
                                                                                                                                                     });

      m_pluginEventHandler.postEvent(kGetDeviceConfigurationSchemaRequest, query);
   }

   void CApiImplementation::processSetDeviceConfiguration(const toPlugin::SetDeviceConfiguration& msg)
   {
      boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration> command = boost::make_shared<CSetDeviceConfiguration>(msg);
      m_pluginEventHandler.postEvent(kSetDeviceConfiguration, command);
   }

   void CApiImplementation::processDeviceCommand(const toPlugin::DeviceCommand& msg)
   {
      boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command = boost::make_shared<CDeviceCommand>(msg);
      m_pluginEventHandler.postEvent(kEventDeviceCommand, command);
   }

   void CApiImplementation::processExtraQuery(const toPlugin::ExtraQuery& msg)
   {
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> command = boost::make_shared<CExtraQuery>(msg,
                                                                                                           [&](const shared::CDataContainer& r)
                                                                                                           {
                                                                                                              toYadoms::msg ans;
                                                                                                              auto answer = ans.mutable_extraqueryanswer();
                                                                                                              answer->set_success(true);
                                                                                                              answer->set_result(r.serialize());
                                                                                                              send(ans);
                                                                                                           },
                                                                                                           [&](const std::string& r)
                                                                                                           {
                                                                                                              toYadoms::msg ans;
                                                                                                              auto answer = ans.mutable_extraqueryanswer();
                                                                                                              answer->set_success(false);
                                                                                                              answer->set_result(r);
                                                                                                              send(ans);
                                                                                                           });
      m_pluginEventHandler.postEvent(kEventExtraQuery, command);
   }

   void CApiImplementation::processManuallyDeviceCreation(const toPlugin::ManuallyDeviceCreation& msg)
   {
      boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request =
         boost::make_shared<CManuallyDeviceCreation>(msg,
                                                     [&](const std::string& newDeviceName)
                                                     {
                                                        toYadoms::msg ans;
                                                        auto answer = ans.mutable_manuallydevicecreationanswer();
                                                        auto success = answer->mutable_sucess();
                                                        success->set_newdevicename(newDeviceName);
                                                        send(ans);
                                                     },
                                                     [&](const std::string& errorMessage)
                                                     {
                                                        toYadoms::msg ans;
                                                        auto answer = ans.mutable_manuallydevicecreationanswer();
                                                        auto error = answer->mutable_error();
                                                        error->set_message(errorMessage);
                                                        send(ans);
                                                     });

      m_pluginEventHandler.postEvent(kEventManuallyDeviceCreation, request);
   }

   void CApiImplementation::processDeviceRemoved(const toPlugin::DeviceRemoved& msg)
   {
      boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event = boost::make_shared<CDeviceRemoved>(msg);
      m_pluginEventHandler.postEvent(kEventDeviceRemoved, event);
   }


   void CApiImplementation::setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                                           const std::string& customMessageId,
                                           const std::map<std::string, std::string>& customMessageDataParams)
   {
      toYadoms::msg req;
      auto request = req.mutable_pluginstate();
      switch (state)
      {
      case shared::plugin::yPluginApi::historization::EPluginState::kUnknownValue: request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kUnknown);
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kErrorValue: request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kError);
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue: request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kStopped);
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kRunningValue: request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kRunning);
         break;
      case shared::plugin::yPluginApi::historization::EPluginState::kCustomValue: request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kCustom);
         break;
      default:
         throw std::out_of_range((boost::format("CApiImplementation::setPluginState, unknown state %1%") % state).str());
      }
      request->set_custommessageid(customMessageId);

      shared::CDataContainer dc(customMessageDataParams);
      request->set_custommessagedata(dc.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : setPluginState(" << state << ", " << customMessageId << ", { ";
         std::for_each(customMessageDataParams.begin(),
                       customMessageDataParams.end(),
                       [](const std::pair<const std::string, std::string>& param)
                       {
                          std::cerr << param.first << " = " << param.second << ", ";
                       });
         std::cerr << "} )" << std::endl;
         throw;
      }
   }

   void CApiImplementation::declareDevice(const std::string& device,
                                          const std::string& model,
                                          boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                                          const shared::CDataContainer& details)
   {
      toYadoms::msg req;
      auto request = req.mutable_declaredevice();
      request->set_device(device);
      request->set_model(model);
      fillHistorizable(keyword, request->add_keywords());
      if (!details.empty())
         request->set_details(details.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : declareDevice(" << device << ", " << model << ", " << keyword->getKeyword() << ", " << details.serialize() << ")" << std::endl;
         throw;
      }
   }

   void CApiImplementation::declareDevice(const std::string& device,
                                          const std::string& model,
                                          const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords,
                                          const shared::CDataContainer& details)
   {
      toYadoms::msg req;
      auto request = req.mutable_declaredevice();
      request->set_device(device);
      request->set_model(model);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         fillHistorizable(*keyword, request->add_keywords());
      if (!details.empty())
         request->set_details(details.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : declareDevice(" << device << ", " << model << ", {";
         std::for_each(keywords.begin(),
                       keywords.end(),
                       [](boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword)
                       {
                          std::cerr << keyword->getKeyword() << ", ";
                       });
         std::cerr << "}, " << details.serialize() << ")" << std::endl;
         throw;
      }
   }

   std::vector<std::string> CApiImplementation::getAllDevices() const
   {
      toYadoms::msg req;
      req.mutable_alldevicesrequest();

      std::vector<std::string> allDevices;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_alldevicesanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 std::copy(ans.alldevicesanswer().devices().begin(), ans.alldevicesanswer().devices().end(), std::back_inserter(allDevices));
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getAllDevices()" << std::endl;
         throw;
      }
      return allDevices;
   }

   bool CApiImplementation::deviceExists(const std::string& device) const
   {
      toYadoms::msg req;
      auto request = req.mutable_deviceexists();
      request->set_device(device);

      auto exists = false;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_deviceexists();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 exists = ans.deviceexists().exists();
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : deviceExists(" << device << ")" << std::endl;
         throw;
      }

      return exists;
   }

   shared::CDataContainer CApiImplementation::getDeviceConfiguration(const std::string& device) const
   {
      toYadoms::msg req;
      auto request = req.mutable_deviceconfigurationrequest();
      request->set_device(device);

      shared::CDataContainer configuration;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_deviceconfigurationanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 configuration.deserialize(ans.deviceconfigurationanswer().configuration());
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getDeviceConfiguration(" << device << ")" << std::endl;
         throw;
      }
      return configuration;
   }

   void CApiImplementation::updateDeviceConfiguration(const std::string& device,
                                                      const shared::CDataContainer& configuration) const
   {
      toYadoms::msg req;
      auto request = req.mutable_updatedeviceconfiguration();
      request->set_device(device);
      request->set_configuration(configuration.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : updateDeviceConfiguration(" << device << ", " << configuration.serialize() << ")" << std::endl;
         throw;
      }
   }

   shared::CDataContainer CApiImplementation::getDeviceDetails(const std::string& device) const
   {
      toYadoms::msg req;
      auto request = req.mutable_devicedetails();
      request->set_device(device);

      shared::CDataContainer details;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_devicedetails();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 details.deserialize(ans.devicedetails().details());
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getDeviceDetails(" << device << ")" << std::endl;
         throw;
      }
      return details;
   }

   void CApiImplementation::updateDeviceDetails(const std::string& device,
                                                const shared::CDataContainer& details) const
   {
      toYadoms::msg req;
      auto request = req.mutable_updatedevicedetails();
      request->set_device(device);
      request->set_details(details.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : updateDeviceDetails(" << device << ", " << details.serialize() << ")" << std::endl;
         throw;
      }
   }

   std::string CApiImplementation::getDeviceModel(const std::string& device) const
   {
      toYadoms::msg req;
      auto request = req.mutable_devicemodelrequest();
      request->set_device(device);

      std::string model;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_devicemodelanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 model = ans.devicemodelanswer().model();
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getDeviceModel(" << device << ")" << std::endl;
         throw;
      }
      return model;
   }

   void CApiImplementation::updateDeviceModel(const std::string& device,
                                              const std::string& model) const
   {
      toYadoms::msg req;
      auto request = req.mutable_updatedevicemodel();
      request->set_device(device);
      request->set_model(model);
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : updateDeviceModel(" << device << ", " << model << ")" << std::endl;
         throw;
      }
   }

   void CApiImplementation::removeDevice(const std::string& device)
   {
      toYadoms::msg req;
      auto request = req.mutable_removedevice();
      request->set_device(device);

      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : removeDevice(" << device << ")" << std::endl;
         throw;
      }
   }

   void CApiImplementation::declareKeywords(const std::string& device,
                                            const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords)
   {
      toYadoms::msg req;
      auto request = req.mutable_declarekeywords();
      request->set_device(device);
      for (auto keyword = keywords.begin(); keyword != keywords.end(); ++keyword)
         fillHistorizable(*keyword, request->add_keywords());

      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : declareKeywords(" << device << ", {";
         std::for_each(keywords.begin(),
                       keywords.end(),
                       [](boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword)
                       {
                          std::cerr << keyword->getKeyword() << ", ";
                       });
         std::cerr << "} )" << std::endl;
         throw;
      }
   }

   void CApiImplementation::declareKeyword(const std::string& device,
                                           boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword,
                                           const shared::CDataContainer& details)
   {
      toYadoms::msg req;
      auto request = req.mutable_declarekeyword();
      request->set_device(device);
      fillHistorizable(keyword, request->mutable_keyword());
      if (!details.empty())
         request->set_details(details.serialize());
      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : declareKeyword(" << device << ", " << keyword << ", " << details.serialize() << ")" << std::endl;
         throw;
      }
   }

   bool CApiImplementation::keywordExists(const std::string& device,
                                          const std::string& keyword) const
   {
      toYadoms::msg req;
      auto request = req.mutable_keywordexists();
      request->set_device(device);
      request->set_keyword(keyword);

      auto exists = false;

      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_keywordexists();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 exists = ans.keywordexists().exists();
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : keywordExists(" << device << ", " << keyword << ")" << std::endl;
         throw;
      }
      return exists;
   }

   bool CApiImplementation::keywordExists(const std::string& device,
                                          boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> keyword) const
   {
      return keywordExists(device, keyword->getKeyword());
   }

   std::vector<std::string> CApiImplementation::getAllKeywords(const std::string& device) const
   {
      toYadoms::msg req;
      auto request = req.mutable_allkeywordsrequest();
      request->set_device(device);

      std::vector<std::string> keywords;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_allkeywordsanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 std::copy(ans.allkeywordsanswer().keywords().begin(), ans.allkeywordsanswer().keywords().end(), std::back_inserter(keywords));
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getAllKeywords(" << device << ")" << std::endl;
         throw;
      }

      return keywords;
   }

   void CApiImplementation::removeKeyword(const std::string& device,
                                          const std::string& keyword)
   {
      toYadoms::msg req;
      auto request = req.mutable_removekeyword();
      request->set_device(device);
      request->set_keyword(keyword);

      try
      {
         send(req);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : removeKeyword(" << keyword << ")" << std::endl;
         throw;
      }
   }

   void CApiImplementation::fillHistorizable(boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> in,
                                             toYadoms::Historizable* out)
   {
      fillCapacity(in->getCapacity(), out->mutable_capacity());
      out->set_accessmode(in->getAccessMode().toString());
      out->set_name(in->getKeyword());
      out->set_type(in->getCapacity().getType().toString());
      out->set_units(in->getCapacity().getUnit());
      out->set_typeinfo(in->getTypeInfo().serialize());
      out->set_measure(in->getMeasureType().toString());
   }

   void CApiImplementation::fillCapacity(const shared::plugin::yPluginApi::CStandardCapacity& in,
                                         toYadoms::Capacity* out)
   {
      out->set_name(in.getName());
      out->set_unit(in.getUnit());
      out->set_type(in.getType().toString());
   }

   std::string CApiImplementation::getRecipientValue(int recipientId,
                                                     const std::string& fieldName) const
   {
      toYadoms::msg req;
      auto request = req.mutable_recipientvaluerequest();
      request->set_recipientid(recipientId);
      request->set_fieldname(fieldName);

      std::string value;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_recipientvalue();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 value = ans.recipientvalue().value();
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getRecipientValue(" << recipientId << ", " << fieldName << ")" << std::endl;
         throw;
      }

      return value;
   }

   std::vector<int> CApiImplementation::findRecipientsFromField(const std::string& fieldName,
                                                                const std::string& expectedFieldValue) const
   {
      toYadoms::msg req;
      auto request = req.mutable_findrecipientsfromfieldrequest();
      request->set_fieldname(fieldName);
      request->set_expectedfieldvalue(expectedFieldValue);

      std::vector<int> recipientIds;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_findrecipientsfromfieldanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 std::copy(ans.findrecipientsfromfieldanswer().recipientids().begin(), ans.findrecipientsfromfieldanswer().recipientids().end(), std::back_inserter(recipientIds));
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : findRecipientsFromField(" << fieldName << ", " << expectedFieldValue << ")" << std::endl;
         throw;
      }

      return recipientIds;
   }

   bool CApiImplementation::recipientFieldExists(const std::string& fieldName) const
   {
      toYadoms::msg req;
      auto request = req.mutable_recipientfieldexitsrequest();
      request->set_fieldname(fieldName);

      auto exists = false;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_keywordexists();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 exists = ans.keywordexists().exists();
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : recipientFieldExists(" << fieldName << ")" << std::endl;
         throw;
      }

      return exists;
   }

   void CApiImplementation::historizeData(const std::string& device,
                                          boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> data)
   {
      toYadoms::msg msg;
      auto message = msg.mutable_historizedata();
      message->set_device(device);
      auto value = message->add_value();
      fillHistorizable(data, value->mutable_historizable());
      value->set_formattedvalue(data->formatValue());
      try
      {
         send(msg);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : historizeData(" << device << ", { " << data->getKeyword() << ", " << data->formatValue() << " } )" << std::endl;
         throw;
      }
   }

   void CApiImplementation::historizeData(const std::string& device,
                                          const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& dataVect)
   {
      toYadoms::msg msg;
      auto message = msg.mutable_historizedata();
      message->set_device(device);
      for (auto data = dataVect.begin(); data != dataVect.end(); ++data)
      {
         auto value = message->add_value();
         fillHistorizable(*data, value->mutable_historizable());
         value->set_formattedvalue((*data)->formatValue());
      }
      try
      {
         send(msg);
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : historizeData(" << device << ", { ";
         std::for_each(dataVect.begin(),
                       dataVect.end(),
                       [](boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable> data)
                       {
                          std::cerr << " { " << data->getKeyword() << ", " << data->formatValue() << " }, ";
                       });
         std::cerr << "} )" << std::endl;
         throw;
      }
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CApiImplementation::getInformation() const
   {
      if (!m_pluginInformation)
         throw std::runtime_error("Plugin information not available");

      return m_pluginInformation;
   }

   shared::CDataContainer CApiImplementation::getConfiguration()
   {
      toYadoms::msg req;
      req.mutable_configurationrequest();

      shared::CDataContainer configuration;
      try
      {
         send(req,
              [](const toPlugin::msg& ans) -> bool
              {
                 return ans.has_configurationanswer();
              },
              [&](const toPlugin::msg& ans) -> void
              {
                 configuration.deserialize(ans.configurationanswer().configuration());
              });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getConfiguration" << std::endl;
         throw;
      }

      return configuration;
   }

   const boost::filesystem::path& CApiImplementation::getDataPath() const
   {
      if (!m_dataPath)
         throw std::runtime_error("Plugin instance data path not available");

      return *m_dataPath;
   }

   shared::event::CEventHandler& CApiImplementation::getEventHandler()
   {
      return m_pluginEventHandler;
   }

   boost::shared_ptr<const shared::plugin::information::IYadomsInformation> CApiImplementation::getYadomsInformation() const
   {
      toYadoms::msg req;
      req.mutable_yadomsinformationrequest();

      boost::shared_ptr<const shared::plugin::information::IYadomsInformation> info;
      try
      {
         send(req,
            [](const toPlugin::msg& ans) -> bool
         {
            return ans.has_yadomsinformationanswer();
         },
            [&](const toPlugin::msg& ans) -> void
         {
            info = boost::make_shared<CYadomsInformation>(ans.yadomsinformationanswer());
         });
      }
      catch (std::exception&)
      {
         std::cerr << "Call was : getYadomsInformation" << std::endl;
         throw;
      }
      return info;
   }
} // namespace plugin_cpp_api	


