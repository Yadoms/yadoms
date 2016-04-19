#include "stdafx.h"
#include "ApiImplementation.h"
#include <shared/DataContainer.h>
#include "PluginInformation.h"
#include <toYadoms.pb.h>


CApiImplementation::CApiImplementation()
   :m_stopRequested(false)
{
}

CApiImplementation::~CApiImplementation()
{
}

bool CApiImplementation::stopRequested() const
{
   return m_stopRequested;
}

//TODO
//void CApiImplementation::sendRequest(const toYadoms::msg& request) const
//{
//   try
//   {
//      if (!request.IsInitialized())
//         throw std::overflow_error("CApiImplementation::sendRequest : request is not fully initialized");
//
//      if (request.ByteSize() > static_cast<int>(m_messageQueueMessageSize))
//         throw std::overflow_error("CApiImplementation::sendRequest : request is too big");
//
//      if (!request.SerializeToArray(m_mqBuffer, m_messageQueueMessageSize))
//         throw std::overflow_error("CApiImplementation::sendRequest : fail to serialize request (too big ?)");
//
//      m_sendMessageQueue->send(m_mqBuffer, request.GetCachedSize(), 0);
//   }
//   catch (boost::interprocess::interprocess_exception& ex)
//   {
//      throw std::overflow_error(std::string("yScriptApiWrapper::sendRequest : Error at IYScriptApi method call, ") + ex.what());
//   }
//}

void CApiImplementation::onReceive(const unsigned char* message, size_t messageSize)
{
   if (messageSize < 1)
      throw std::runtime_error("CApiImplementation::processMessage : received Yadoms answer is zero length");

   toPlugin::msg toPluginProtoBuffer;
   if (!toPluginProtoBuffer.ParseFromArray(message, messageSize))
      throw shared::exception::CInvalidParameter("message");

   switch (toPluginProtoBuffer.OneOf_case())
   {
   case toPlugin::msg::kSystem: processSystem(toPluginProtoBuffer.system()); break;
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

void CApiImplementation::setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId)
{}
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
const shared::plugin::information::IInformation& CApiImplementation::getInformation() const
{
   toYadoms::msg req;
   auto request = req.mutable_getplugininformation();
   sendRequest(req);

   //TODO attendre la réponse sans négliger que d'autres messages puissent arriver

   auto information(boost::make_shared<CPluginInformation>());
   boost::shared_ptr<shared::plugin::information::IInformation> i;
   return *i;
}
shared::CDataContainer CApiImplementation::getConfiguration() const
{
   return shared::CDataContainer();
}
shared::event::CEventHandler & CApiImplementation::getEventHandler()
{
   return shared::event::CEventHandler();
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