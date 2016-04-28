#include "stdafx.h"
#include "ApiImplementation.h"
#include <shared/DataContainer.h>
#include "PluginInformation.h"
#include "BindingQuery.h"


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

void CApiImplementation::send(const toYadoms::msg& msg)
{
   try
   {
      if (!m_sendMessageQueue)
         throw std::runtime_error("CApiImplementation::send : plugin API not ready to send message");

      if (!msg.IsInitialized())
         throw std::overflow_error("CApiImplementation::sendRequest : request is not fully initialized");

      if (msg.ByteSize() > static_cast<int>(m_sendMessageQueue->get_max_msg_size()))
         throw std::overflow_error("CApiImplementation::sendRequest : request is too big");

      if (!msg.SerializeToArray(m_mqBuffer.get(), m_sendMessageQueue->get_max_msg_size()))
         throw std::overflow_error("CApiImplementation::sendRequest : fail to serialize request (too big ?)");

      m_sendMessageQueue->send(m_mqBuffer.get(), msg.GetCachedSize(), 0);
   }
   catch (boost::interprocess::interprocess_exception& ex)
   {
      throw std::overflow_error((boost::format("CApiImplementation::send : %1%") % ex.what()).str());
   }
}

void CApiImplementation::send(const toYadoms::msg& msg,
                              boost::function1<bool, const toPlugin::msg&> checkExpectedMessageFunction,
                              boost::function1<void, const toPlugin::msg&> onReceiveFunction)
{
   shared::event::CEventHandler receivedEvtHandler;

   {
      boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
      m_onReceiveHook = [&](const toPlugin::msg& receivedMsg)->bool
      {
         if (!checkExpectedMessageFunction(receivedMsg))
            return false;

         receivedEvtHandler.postEvent<const toPlugin::msg>(shared::event::kUserFirstId, receivedMsg);
         return true;
      };
   }

   send(msg);

   if (receivedEvtHandler.waitForEvents(boost::posix_time::seconds(10)) == shared::event::kTimeout)
      throw std::runtime_error((boost::format("No answer from Yadoms when sending message %1%") % msg.OneOf_case()).str());

   onReceiveFunction(receivedEvtHandler.getEventData<const toPlugin::msg>());
}

void CApiImplementation::onReceive(boost::shared_ptr<const unsigned char[]> message, size_t messageSize)
{
   if (messageSize < 1)
      throw std::runtime_error("CApiImplementation::onReceive : received Yadoms answer is zero length");

   toPlugin::msg toPluginProtoBuffer;
   if (!toPluginProtoBuffer.ParseFromArray(message.get(), messageSize))
      throw shared::exception::CInvalidParameter("message");

   if (!m_initialized) //TODO voir si possible utiliser les hook plutôt que de faire un cas particulier
   {
      switch (toPluginProtoBuffer.OneOf_case())
      {
      case toPlugin::msg::kPluginInformation: processPluginInformation(toPluginProtoBuffer.plugininformation()); break;
      default:
         throw shared::exception::CInvalidParameter((boost::format("Unexpected message %1% when initialization") % toPluginProtoBuffer.OneOf_case()).str());
      }

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
   case toPlugin::msg::kSystem: processSystem(toPluginProtoBuffer.system()); break;
   case toPlugin::msg::kPluginInformation: processPluginInformation(toPluginProtoBuffer.plugininformation()); break;
      //TODO ajouter toPlugin::msg::kConfiguration
   case toPlugin::msg::kBindingQuery: processBindingQuery(toPluginProtoBuffer.bindingquery()); break;
      //TODO
   //case toPlugin::msg::kGetKeywordId: processGetKeywordId(toPluginProtoBuffer.getkeywordid(), messageQueue); break;
   //case toPlugin::msg::kGetRecipientId: processGetRecipientId(toPluginProtoBuffer.getrecipientid(), messageQueue); break;
   //case toPlugin::msg::kReadKeyword: processReadKeyword(toPluginProtoBuffer.readkeyword(), messageQueue); break;
   //case toPlugin::msg::kWaitForNextAcquisition: processWaitForNextAcquisition(toPluginProtoBuffer.waitfornextacquisition(), messageQueue); break;
   //case toPlugin::msg::kWaitForNextAcquisitions: processWaitForNextAcquisitions(toPluginProtoBuffer.waitfornextacquisitions(), messageQueue); break;
   //case toPlugin::msg::kWaitForEvent: processWaitForEvent(toPluginProtoBuffer.waitforevent(), messageQueue); break;
   //case toPlugin::msg::kWriteKeyword: processWriteKeyword(toPluginProtoBuffer.writekeyword(), messageQueue); break;
   //case toPlugin::msg::kSendNotification: processSendNotification(toPluginProtoBuffer.sendnotification(), messageQueue); break;
   //case toPlugin::msg::kGetInfo: processGetInfo(toPluginProtoBuffer.getinfo(), messageQueue); break;
   default:
      throw shared::exception::CInvalidParameter("message");
   }
}


void CApiImplementation::waitInitialized() const
{
   std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
   if(!m_initialized)
      m_initializationCondition.wait(lock);
}

void CApiImplementation::processSystem(const toPlugin::System& msg)
{
   if (msg.type() == toPlugin::System_EventType_kRequestStop)
   {
      // Request the main thread to stop
      getEventHandler().postEvent(kEventStopRequested);

      // Stop this thread
      throw boost::thread_interrupted();
   }
}

void CApiImplementation::processPluginInformation(const toPlugin::Information& msg)
{
   m_pluginInformation = boost::make_shared<CPluginInformation>(boost::make_shared<const toPlugin::Information>(msg));
   setInitialized();
}

void CApiImplementation::setInitialized()
{
   if (!!m_pluginInformation)
   {
      std::unique_lock<std::mutex> lock(m_initializationConditionMutex);
      m_initialized = true;
      m_initializationCondition.notify_one();
   }
}

void CApiImplementation::processBindingQuery(const toPlugin::BindingQuery& msg)
{
   bool success;
   std::string result;

   CBindingQuery query(msg,
                       getEventHandler(),
                       [&](const shared::CDataContainer& r)
                       {
                          success = true;
                          result = r.serialize();
                       },
                       [&](const std::string& r)
                       {
                          success = false;
                          result = r;
                       });
   query.askToPlugin();

   toYadoms::msg ans;
   auto answer = ans.mutable_bindingqueryanswer();
   answer->set_success(success);
   answer->set_result(result);
   send(ans);
}

void CApiImplementation::setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId)
{
   toYadoms::msg req;
   auto request = req.mutable_pluginstate();
   switch(state)
   {
   case shared::plugin::yPluginApi::historization::EPluginState::kUnknownValue:request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kUnknown); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kErrorValue:request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kError); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue:request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kStopped); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kRunningValue:request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kRunning); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kCustomValue:request->set_pluginstate(toYadoms::SetPluginState_EPluginState_kCustom); break;
   default:
      throw std::out_of_range((boost::format("CApiImplementation::setPluginState, unknown state %1%") % state).str());
   }
   request->set_custommessageid(customMessageId);
   send(req);
}
bool CApiImplementation::deviceExists(const std::string& device) const
{
   return false;
}
shared::CDataContainer CApiImplementation::getDeviceDetails(const std::string& device) const
{
   return shared::CDataContainer();
}
void CApiImplementation::declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details)
{}
bool CApiImplementation::keywordExists(const std::string& device, const std::string& keyword) const
{
   return false;
}
bool CApiImplementation::keywordExists(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword) const
{
   return false;
}
void CApiImplementation::declareKeyword(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details)
{}
std::string CApiImplementation::getRecipientValue(int recipientId, const std::string& fieldName) const
{
   return std::string();
}
std::vector<int> CApiImplementation::findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const
{
   return std::vector<int>();
}
bool CApiImplementation::recipientFieldExists(const std::string& fieldName) const
{
   return false;
}
void CApiImplementation::historizeData(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& data)
{}
void CApiImplementation::historizeData(const std::string& device, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect)
{}
boost::shared_ptr<const shared::plugin::information::IInformation> CApiImplementation::getInformation() const
{
   if (!m_pluginInformation)
      throw std::exception("Plugin information not available");

   return m_pluginInformation;
}

shared::CDataContainer CApiImplementation::getConfiguration()
{
   toYadoms::msg req;
   req.mutable_configurationrequest();

   shared::CDataContainer configuration;
   send(req,
        [&](const toPlugin::msg& ans) -> bool
        {
           return ans.has_configurationanswer();
        },
        [&](const toPlugin::msg& ans) -> void
        {
           configuration.deserialize(ans.configurationanswer().configuration());
        });

   return configuration;
}

shared::event::CEventHandler& CApiImplementation::getEventHandler()
{
   return m_pluginEventHandler;
}

//int CApiImplementation::getKeywordId(const std::string& deviceName, const std::string& keywordName) const
//{
//   pbRequest::msg req;
//   pbRequest::GetKeywordId* request = req.mutable_getkeywordid();
//   request->set_devicename(deviceName);
//   request->set_keywordname(keywordName);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_getkeywordid())
//      throw std::out_of_range("yScriptApiWrapper::getKeywordId, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::getKeywordId, error : ") + answer.error());
//
//   return answer.getkeywordid().id();
//}
//
//int CApiImplementation::getRecipientId(const std::string& firstName, const std::string& lastName) const
//{
//   pbRequest::msg req;
//   pbRequest::GetRecipientId* request = req.mutable_getrecipientid();
//   request->set_firstname(firstName);
//   request->set_lastname(lastName);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_getrecipientid())
//      throw std::out_of_range("yScriptApiWrapper::getRecipientId, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::getRecipientId, error : ") + answer.error());
//
//   return answer.getrecipientid().id();
//}
//
//std::string CApiImplementation::readKeyword(int keywordId) const
//{
//   pbRequest::msg req;
//   pbRequest::ReadKeyword* request = req.mutable_readkeyword();
//   request->set_keywordid(keywordId);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_readkeyword())
//      throw std::out_of_range("yScriptApiWrapper::readKeyword, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::readKeyword, error : ") + answer.error());
//
//   return answer.readkeyword().value();
//}
//
//std::string CApiImplementation::waitForNextAcquisition(int keywordId, const std::string& timeout) const
//{
//   pbRequest::msg req;
//   pbRequest::WaitForNextAcquisition* request = req.mutable_waitfornextacquisition();
//   request->set_keywordid(keywordId);
//   if (!timeout.empty())
//      request->set_timeout(timeout);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_waitfornextacquisition())
//      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisition, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisition, error : ") + answer.error());
//
//   return answer.waitfornextacquisition().acquisition();
//}
//
//std::pair<int, std::string> CApiImplementation::waitForNextAcquisitions(const std::vector<int> & keywordIdList, const std::string& timeout) const
//{
//   pbRequest::msg req;
//   pbRequest::WaitForNextAcquisitions* request = req.mutable_waitfornextacquisitions();
//   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
//      request->add_keywordid(*it);
//   if (!timeout.empty())
//      request->set_timeout(timeout);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_waitfornextacquisitions())
//      throw std::out_of_range("yScriptApiWrapper::waitForNextAcquisitions, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::waitForNextAcquisitions, error : ") + answer.error());
//
//   return std::pair<int, std::string>(answer.waitfornextacquisitions().keywordid(), answer.waitfornextacquisitions().has_acquisition() ? answer.waitfornextacquisitions().acquisition() : std::string());
//}
//
//shared::script::yScriptApi::CWaitForEventResult CApiImplementation::waitForEvent(const std::vector<int> & keywordIdList, bool receiveDateTimeEvent, const std::string& timeout) const
//{
//   pbRequest::msg req;
//   pbRequest::WaitForEvent* request = req.mutable_waitforevent();
//   for (std::vector<int>::const_iterator it = keywordIdList.begin(); it != keywordIdList.end(); ++it)
//      request->add_keywordid(*it);
//   request->set_receivedatetimeevent(receiveDateTimeEvent);
//   if (!timeout.empty())
//      request->set_timeout(timeout);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_waitforevent())
//      throw std::out_of_range("yScriptApiWrapper::waitForEvent, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::waitForEvent, error : ") + answer.error());
//
//   shared::script::yScriptApi::CWaitForEventResult result;
//   switch (answer.waitforevent().type())
//   {
//   case pbAnswer::WaitForEvent_EventType_kTimeout : result.setType(shared::script::yScriptApi::CWaitForEventResult::kTimeout); break;
//   case pbAnswer::WaitForEvent_EventType_kKeyword : result.setType(shared::script::yScriptApi::CWaitForEventResult::kKeyword); break;
//   case pbAnswer::WaitForEvent_EventType_kDateTime : result.setType(shared::script::yScriptApi::CWaitForEventResult::kDateTime); break;
//   default:
//      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
//   }
//
//   result.setKeywordId(answer.waitforevent().keywordid());
//   result.setValue(answer.waitforevent().has_acquisition() ? answer.waitforevent().acquisition() : std::string());
//   return result;
//}
//
//void CApiImplementation::writeKeyword(int keywordId, const std::string& newState)
//{
//   pbRequest::msg req;
//   pbRequest::WriteKeyword* request = req.mutable_writekeyword();
//   request->set_keywordid(keywordId);
//   request->set_newstate(newState);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_writekeyword())
//      throw std::out_of_range("yScriptApiWrapper::writeKeyword, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::writeKeyword, error : ") + answer.error());
//}
//
//void CApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
//{
//   pbRequest::msg req;
//   pbRequest::SendNotification* request = req.mutable_sendnotification();
//   request->set_keywordid(keywordId);
//   request->set_recipientid(recipientId);
//   request->set_message(message);
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_sendnotification())
//      throw std::out_of_range("yScriptApiWrapper::sendNotification, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::sendNotification, error : ") + answer.error());
//}
//
//std::string CApiImplementation::getInfo(EInfoKeys key) const
//{
//   pbRequest::msg req;
//   pbRequest::GetInfo* request = req.mutable_getinfo();
//   switch (key)
//   {
//   case kSunrise:request->set_key(pbRequest::GetInfo_Key_kSunrise); break;
//   case kSunset:request->set_key(pbRequest::GetInfo_Key_kSunset); break;
//   case kLatitude:request->set_key(pbRequest::GetInfo_Key_kLatitude); break;
//   case kLongitude:request->set_key(pbRequest::GetInfo_Key_kLongitude); break;
//   case kAltitude:request->set_key(pbRequest::GetInfo_Key_kAltitude); break;
//   case kYadomsServerOS:request->set_key(pbRequest::GetInfo_Key_kYadomsServerOS); break;
//   case kYadomsServerVersion:request->set_key(pbRequest::GetInfo_Key_kYadomsServerVersion); break;
//   default:
//      throw shared::exception::CInvalidParameter("answer.waitforevent.type");
//   }
//   sendRequest(req);
//
//   pbAnswer::msg answer;
//   receiveAnswer(answer);
//
//   if (!answer.has_getinfo())
//      throw std::out_of_range("yScriptApiWrapper::getInfo, wrong message received");
//
//   if (answer.has_error())
//      throw std::out_of_range(std::string("yScriptApiWrapper::getInfo, error : ") + answer.error());
//
//   return answer.getinfo().value();
//}