#include "stdafx.h"
#include "YadomsApiImplementation.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>

namespace pluginSystem
{

CYadomsApiImplementation::CYadomsApiImplementation(
   boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
   const boost::filesystem::path libraryPath, 
   const boost::shared_ptr<database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<database::IKeywordRequester> keywordRequester,
   boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer)
   :m_informations(pluginInformations), m_libraryPath(libraryPath),  m_pluginData(pluginData), m_pluginEventLoggerRequester(pluginEventLoggerRequester),
   m_deviceManager(deviceManager), m_keywordRequester(keywordRequester), m_acquisitionRequester(acquisitionRequester), m_acquisitionHistorizer(acquisitionHistorizer)
{

}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string& device) const
{
   return m_deviceManager->deviceExists(getPluginId(), device);
}

const shared::CDataContainer CYadomsApiImplementation::getDeviceDetails(const std::string& device) const
{
   return m_deviceManager->getDevice(getPluginId(), device)->Details;
}

void CYadomsApiImplementation::declareDevice(const std::string& device, const std::string& model, const shared::CDataContainer& details)
{
   if (deviceExists(device))
      throw shared::exception::CEmptyResult((boost::format("Error declaring device %1% : already exists") % device).str());

   m_deviceManager->createDevice(getPluginId(), device, device, model, details);
}

bool CYadomsApiImplementation::keywordExists(const std::string& device, const std::string& keyword) const
{
   if (!deviceExists(device))
      throw shared::exception::CEmptyResult("Fail to check if keyword exists : owner device doesn't exist.");

   return m_keywordRequester->keywordExists((m_deviceManager->getDevice(getPluginId(), device))->Id, keyword);
}

bool CYadomsApiImplementation::keywordExists(const std::string& device, const shared::plugin::yadomsApi::historization::IHistorizable& keyword) const
{
   return keywordExists(device, keyword.getKeyword());
}

void CYadomsApiImplementation::declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity,
   const shared::plugin::yadomsApi::EKeywordAccessMode& accessMode, const shared::plugin::yadomsApi::EKeywordDataType& type, const std::string & units,
   const shared::plugin::yadomsApi::historization::EMeasureType & measure, const shared::CDataContainer& details)
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
   keywordEntity.Details = details;

   m_keywordRequester->addKeyword(keywordEntity);
}

void CYadomsApiImplementation::declareKeyword(const std::string& device, const shared::plugin::yadomsApi::historization::IHistorizable& keyword, const shared::CDataContainer& details)
{
   declareCustomKeyword(device, keyword.getKeyword(), keyword.getCapacity().getName(), keyword.getAccessMode(), keyword.getCapacity().getType(), keyword.getCapacity().getUnit(), keyword.getMeasureType(), details);
}



void CYadomsApiImplementation::historizeData(const std::string& device, const shared::plugin::yadomsApi::historization::IHistorizable& data)
{
   try
   {
      boost::shared_ptr<database::entities::CDevice> deviceEntity = m_deviceManager->getDevice(getPluginId(), device);
      boost::shared_ptr<database::entities::CKeyword> keywordEntity = m_keywordRequester->getKeyword(deviceEntity->Id, data.getKeyword());
      m_acquisitionHistorizer->saveData(keywordEntity->Id, data);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
   }
}

const shared::plugin::information::IInformation& CYadomsApiImplementation::getInformation() const
{
   return *m_informations;
}

shared::CDataContainer CYadomsApiImplementation::getConfiguration() const
{
   return m_pluginData->Configuration;
}
        
void CYadomsApiImplementation::recordPluginEvent(PluginEventSeverity severity, const std::string& message)
{
   database::entities::EEventType evenType;
   switch(severity)
   {
   case kError: evenType = database::entities::EEventType::kError; break;
   case kInfo: evenType = database::entities::EEventType::kInfo; break;
   default:
      {
         BOOST_ASSERT_MSG(false, "Unknown plugin event severity type");
         evenType = database::entities::EEventType::kInfo; // Set a default value
         break;
      }
   }
   m_pluginEventLoggerRequester->addEvent(m_informations->getName(), m_informations->getVersion(), m_informations->getReleaseType(), evenType, message);
}

shared::event::CEventHandler & CYadomsApiImplementation::getEventHandler()
{
   return m_pluginEventHandler;
}

int CYadomsApiImplementation::getPluginId() const
{
   return m_pluginData->Id;
}
	
const boost::filesystem::path CYadomsApiImplementation::getPluginPath() const
{
   return m_libraryPath;
}

} // namespace pluginSystem	
