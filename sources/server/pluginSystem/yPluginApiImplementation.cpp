#include "stdafx.h"
#include "yPluginApiImplementation.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace pluginSystem
{

CYPluginApiImplementation::CYPluginApiImplementation(
   boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
   const boost::shared_ptr<const database::entities::CPlugin> instanceData,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer)
   :m_informations(pluginInformations),
   m_instanceData(instanceData),
   m_pluginEventLoggerRequester(dataProvider->getPluginEventLoggerRequester()),
   m_deviceManager(deviceManager),
   m_keywordRequester(dataProvider->getKeywordRequester()),
   m_recipientRequester(dataProvider->getRecipientRequester()),
   m_acquisitionRequester(dataProvider->getAcquisitionRequester()),
   m_acquisitionHistorizer(acquisitionHistorizer)   
{
}
      
CYPluginApiImplementation::~CYPluginApiImplementation() 
{
}

void CYPluginApiImplementation::setPluginState(const shared::plugin::yPluginApi::historization::EPluginState& state, const std::string & customMessageId)
{
   if (!customMessageId.empty() && (state != shared::plugin::yPluginApi::historization::EPluginState::kCustom && state != shared::plugin::yPluginApi::historization::EPluginState::kError))
      YADOMS_LOG(warning) << "Custom message ID \"" << customMessageId << "\" will be ignored as state is " << state.toString();

   pluginStateKeyword()->set(state);
   pluginStateMessageIdKeyword()->set(customMessageId);
   historizeData(pluginStateDevice(), *pluginStateKeyword());
   historizeData(pluginStateDevice(), *pluginStateMessageIdKeyword());

   switch (state)
   {
   case shared::plugin::yPluginApi::historization::EPluginState::kErrorValue: recordPluginEvent(PluginEventSeverity::kError, "error"); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kStoppedValue: recordPluginEvent(PluginEventSeverity::kInfo, "stopped"); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kRunningValue: recordPluginEvent(PluginEventSeverity::kInfo, "started"); break;
   case shared::plugin::yPluginApi::historization::EPluginState::kCustomValue: recordPluginEvent(PluginEventSeverity::kInfo, std::string("custom event (") + customMessageId + std::string(")")); break;
   default: break;
   }
}


bool CYPluginApiImplementation::deviceExists(const std::string& device) const
{
   return m_deviceManager->deviceExists(getPluginId(), device);
}

shared::CDataContainer CYPluginApiImplementation::getDeviceDetails(const std::string& device) const
{
   return m_deviceManager->getDevice(getPluginId(), device)->Details;
}

void CYPluginApiImplementation::declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details)
{
   if (deviceExists(device))
      throw shared::exception::CEmptyResult((boost::format("Error declaring device %1% : already exists") % device).str());

   m_deviceManager->createDevice(getPluginId(), device, device, model, details);
}

bool CYPluginApiImplementation::keywordExists(const std::string& device, const std::string& keyword) const
{
   if (!deviceExists(device))
      throw shared::exception::CEmptyResult("Fail to check if keyword exists : owner device doesn't exist.");

   return m_keywordRequester->keywordExists((m_deviceManager->getDevice(getPluginId(), device))->Id, keyword);
}

bool CYPluginApiImplementation::keywordExists(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword) const
{
   return keywordExists(device, keyword.getKeyword());
}

void CYPluginApiImplementation::declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity,
   const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode, const shared::plugin::yPluginApi::EKeywordDataType& type, const std::string & units,
   const shared::plugin::yPluginApi::historization::EMeasureType & measure, const shared::CDataContainer& typeInfo, const shared::CDataContainer& details)
{
   if (keywordExists(device, keyword))
      throw shared::exception::CEmptyResult("Fail to declare keyword : keyword already exists");

   database::entities::CKeyword keywordEntity;
   keywordEntity.DeviceId = m_deviceManager->getDevice(getPluginId(), device)->Id;
   keywordEntity.CapacityName = capacity;
   keywordEntity.AccessMode = accessMode;
   keywordEntity.Type = type;
   keywordEntity.Units = units;
   keywordEntity.Name = keyword;
   keywordEntity.FriendlyName = keyword;
   keywordEntity.Measure = measure;
   keywordEntity.TypeInfo = typeInfo;
   keywordEntity.Details = details;

   m_keywordRequester->addKeyword(keywordEntity);
}

std::string CYPluginApiImplementation::pluginStateDevice()
{
   static const std::string PluginStateDeviceName("pluginState");

   if (!deviceExists(PluginStateDeviceName))
      m_deviceManager->createDevice(getPluginId(), PluginStateDeviceName, m_instanceData->DisplayName() + " plugin state", "Plugin state", shared::CDataContainer::EmptyContainer);

   return PluginStateDeviceName;
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::CPluginState> CYPluginApiImplementation::pluginStateKeyword()
{
   if (!m_pluginStateKeyword)
   {
      m_pluginStateKeyword = boost::make_shared <shared::plugin::yPluginApi::historization::CPluginState>("state");

      if (!keywordExists(pluginStateDevice(), *m_pluginStateKeyword))
         declareKeyword(pluginStateDevice(), *m_pluginStateKeyword);
   }

   return m_pluginStateKeyword;
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::CText> CYPluginApiImplementation::pluginStateMessageIdKeyword()
{
   if (!m_pluginStateMessageIdKeyword)
   {
      m_pluginStateMessageIdKeyword = boost::make_shared <shared::plugin::yPluginApi::historization::CText>("customMessageId");

      if (!keywordExists(pluginStateDevice(), *m_pluginStateMessageIdKeyword))
         declareKeyword(pluginStateDevice(), *m_pluginStateMessageIdKeyword);
   }

   return m_pluginStateMessageIdKeyword;
}

void CYPluginApiImplementation::declareKeyword(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& keyword, const shared::CDataContainer& details)
{
   declareCustomKeyword(device, keyword.getKeyword(), keyword.getCapacity().getName(), keyword.getAccessMode(), keyword.getCapacity().getType(), keyword.getCapacity().getUnit(), keyword.getMeasureType(), keyword.getTypeInfo(), details);
}

std::string CYPluginApiImplementation::getRecipientValue(int recipientId, const std::string& fieldName) const
{
   boost::shared_ptr<const database::entities::CRecipient> recipient = m_recipientRequester->getRecipient(recipientId);

   // Search for from plugin fields
   for (std::vector<boost::shared_ptr<database::entities::CRecipientField> >::const_iterator itField = recipient->Fields().begin(); itField != recipient->Fields().end(); ++itField)
      if ((*itField)->PluginType == m_instanceData->Type && (*itField)->FieldName == fieldName)
         return (*itField)->Value;

   // If not found from plugin fields, looking for general fields
   for (std::vector<boost::shared_ptr<database::entities::CRecipientField> >::const_iterator itField = recipient->Fields().begin(); itField != recipient->Fields().end(); ++itField)
      if ((*itField)->PluginType == "system" && (*itField)->FieldName == fieldName)
         return (*itField)->Value;

   throw shared::exception::CEmptyResult((boost::format("Cannot retrieve field %1% for recipient Id %2% in database") % fieldName % recipientId).str());
}

std::vector<int> CYPluginApiImplementation::findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue) const
{
   std::vector<boost::shared_ptr<database::entities::CRecipient> > recipients = m_recipientRequester->findRecipientsFromField(fieldName, expectedFieldValue);
   std::vector<int> recipientIds;

   for (std::vector<boost::shared_ptr<database::entities::CRecipient> >::const_iterator itRecipient = recipients.begin(); itRecipient != recipients.end(); ++itRecipient)
      recipientIds.push_back((*itRecipient)->Id);

   return recipientIds;
}

bool CYPluginApiImplementation::recipientFieldExists(const std::string& fieldName) const
{
   return m_recipientRequester->fieldExists(fieldName, getInformation().getType());
}



void CYPluginApiImplementation::historizeData(const std::string& device, const shared::plugin::yPluginApi::historization::IHistorizable& data)
{
   try
   {
      boost::shared_ptr<const database::entities::CDevice> deviceEntity = m_deviceManager->getDevice(getPluginId(), device);
      boost::shared_ptr<const database::entities::CKeyword> keywordEntity = m_keywordRequester->getKeyword(deviceEntity->Id, data.getKeyword());

      m_acquisitionHistorizer->saveData(keywordEntity->Id, data);

   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
   }
}

void CYPluginApiImplementation::historizeData(const std::string& device, const std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > & dataVect)
{
   try
   {
	   std::vector<int> keywordIdList;
	   std::vector<boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> >::const_iterator iter;

      boost::shared_ptr<const database::entities::CDevice> deviceEntity = m_deviceManager->getDevice(getPluginId(), device);
      for (iter = dataVect.begin(); iter != dataVect.end(); ++iter)
      {
         boost::shared_ptr<const database::entities::CKeyword> keywordEntity = m_keywordRequester->getKeyword(deviceEntity->Id, (*iter)->getKeyword());
         keywordIdList.push_back(keywordEntity->Id);
      }
      m_acquisitionHistorizer->saveData(keywordIdList, dataVect);

   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
   }
}

const shared::plugin::information::IInformation& CYPluginApiImplementation::getInformation() const
{
   return *m_informations;
}

shared::CDataContainer CYPluginApiImplementation::getConfiguration() const
{
   return m_instanceData->Configuration;
}
        
void CYPluginApiImplementation::recordPluginEvent(PluginEventSeverity severity, const std::string& message)
{
   database::entities::EEventType evenType;
   switch (severity)
   {
   case kError: evenType = database::entities::EEventType::kError; break;
   case kInfo: evenType = database::entities::EEventType::kInfo; break;
   default:
      {
         YADOMS_LOG(warning) << "Unknown plugin event severity type " << severity;
         evenType = database::entities::EEventType::kInfo; // Set a default value
         break;
      }
   }
   m_pluginEventLoggerRequester->addEvent(m_informations->getType(), m_informations->getVersion(), m_informations->getReleaseType(), evenType, message);
}

shared::event::CEventHandler & CYPluginApiImplementation::getEventHandler()
{
   return m_pluginEventHandler;
}

int CYPluginApiImplementation::getPluginId() const
{
   return m_instanceData->Id;
}

} // namespace pluginSystem	
