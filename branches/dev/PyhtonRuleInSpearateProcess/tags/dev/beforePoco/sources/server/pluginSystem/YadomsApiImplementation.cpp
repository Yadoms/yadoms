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
   boost::shared_ptr<database::IDeviceRequester> deviceRequester,
   boost::shared_ptr<database::IKeywordRequester> keywordRequester,
   boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester)
   :m_informations(pluginInformations), m_libraryPath(libraryPath),  m_pluginData(pluginData), m_pluginEventLoggerRequester(pluginEventLoggerRequester),
   m_deviceRequester(deviceRequester), m_keywordRequester(keywordRequester), m_acquisitionRequester(acquisitionRequester)
{

}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string& device) const
{
   return m_deviceRequester->deviceExists(getPluginId(), device);
}

const shared::CDataContainer CYadomsApiImplementation::getDeviceDetails(const std::string& device) const
{
   return m_deviceRequester->getDevice(getPluginId(), device)->Details;
}

bool CYadomsApiImplementation::declareDevice(const std::string& device, const std::string& model, const std::string & details)
{
   if (deviceExists(device))
      return false;

   m_deviceRequester->createDevice(getPluginId(), device, device, model, details);
   return true;
}

bool CYadomsApiImplementation::keywordExists(const std::string& device, const std::string& keyword) const
{
   if (!deviceExists(device))
      throw shared::exception::CEmptyResult("Fail to declare keyword : owner device doesn't exist.");

   return m_keywordRequester->keywordExists((m_deviceRequester->getDevice(getPluginId(), device))->Id, keyword);
}


bool CYadomsApiImplementation::declareCustomKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::EKeywordType type, const std::string & units, const shared::CDataContainer& details)
{
   if (keywordExists(device, keyword))
      return false;

   database::entities::CKeyword keywordEntity;
   keywordEntity.DeviceId = m_deviceRequester->getDevice(getPluginId(), device)->Id;
   keywordEntity.CapacityName = capacity;
   switch(accessMode)
   {
   case shared::plugin::yadomsApi::kGetSet: keywordEntity.AccessMode = database::entities::EKeywordAccessMode::kGetSet; break;
   case shared::plugin::yadomsApi::kGet: keywordEntity.AccessMode = database::entities::EKeywordAccessMode::kGet; break;
   case shared::plugin::yadomsApi::kSet: keywordEntity.AccessMode = database::entities::EKeywordAccessMode::kSet; break;
   default:
      BOOST_ASSERT_MSG(false, "Unknown accessMode");
      throw shared::exception::CEmptyResult("Fail to declare keyword : unknown accessMode");
      break;
   }

   switch (type)
   {
   case shared::plugin::yadomsApi::kNoData: keywordEntity.Type = database::entities::EKeywordDataType::kNoData; break;
   case shared::plugin::yadomsApi::kString: keywordEntity.Type = database::entities::EKeywordDataType::kString; break;
   case shared::plugin::yadomsApi::kNumeric: keywordEntity.Type = database::entities::EKeywordDataType::kNumeric; break;
   case shared::plugin::yadomsApi::kBool: keywordEntity.Type = database::entities::EKeywordDataType::kBool; break;
   case shared::plugin::yadomsApi::kJson: keywordEntity.Type = database::entities::EKeywordDataType::kJson; break;
   default:
      BOOST_ASSERT_MSG(false, "Unknown type");
      throw shared::exception::CEmptyResult("Fail to declare keyword : unknown type");
      break;
   }

   keywordEntity.Units = units;
   keywordEntity.Name = keyword;
   keywordEntity.FriendlyName = keyword;


   keywordEntity.Details = details;

   m_keywordRequester->addKeyword(keywordEntity);

   return true;
}
      
bool CYadomsApiImplementation::declareKeyword(const std::string& device, const std::string& keyword, const shared::plugin::yadomsApi::CStandardCapacity & capacity, const shared::CDataContainer& details)
{
   return declareCustomKeyword(device, keyword, capacity.getName(), capacity.getAccessMode(), capacity.getType(), capacity.getUnit(), details);
}



void CYadomsApiImplementation::historizeData(const std::string& device, const std::string& keyword, const std::string& value)
{
   try
   {
      boost::shared_ptr<database::entities::CDevice> deviceEntity = m_deviceRequester->getDevice(getPluginId(), device);
      boost::shared_ptr<database::entities::CKeyword> keywordEntity = m_keywordRequester->getKeyword(deviceEntity->Id, keyword);

      m_acquisitionRequester->saveData(keywordEntity->Id, value);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Error historizing data, device or keyword not found : " << e.what();
   }
}

void CYadomsApiImplementation::historizeData(const std::string& device, const std::string& keyword, bool value)
{
   historizeData(device, keyword, boost::lexical_cast<std::string>(value));
}  

void CYadomsApiImplementation::historizeData(const std::string& device, const std::string& keyword, int value)
{
   historizeData(device, keyword, boost::lexical_cast<std::string>(value));
}  

void CYadomsApiImplementation::historizeData(const std::string& device, const std::string& keyword, double value)
{
   historizeData(device, keyword, boost::lexical_cast<std::string>(value));
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
         BOOST_ASSERT_MSG(false, "Unkown plugin event severity type");
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
