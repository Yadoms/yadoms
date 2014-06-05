#include "stdafx.h"
#include "YadomsApiImplementation.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>

namespace pluginSystem
{

CYadomsApiImplementation::CYadomsApiImplementation(
   boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformations,
   const boost::shared_ptr<database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IDeviceRequester> deviceRequester,
   boost::shared_ptr<database::IKeywordRequester> keywordRequester,
   boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester)
   :m_informations(pluginInformations), m_pluginData(pluginData), m_deviceRequester(deviceRequester), m_keywordRequester(keywordRequester), m_acquisitionRequester(acquisitionRequester)
{
}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string& device) const
{
   return m_deviceRequester->deviceExists(getPluginId(), device);
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

bool CYadomsApiImplementation::declareKeyword(const std::string& device, const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, const std::string& details)
{
   if (keywordExists(device, keyword))
      return false;

   database::entities::CKeyword keywordEntity;
   keywordEntity.DeviceId = m_deviceRequester->getDevice(getPluginId(), device)->Id;
   keywordEntity.CapacityName = capacity;
   switch(accessMode)
   {
   case shared::plugin::yadomsApi::IYadomsApi::kReadWrite: keywordEntity.AccessMode = database::entities::kReadWrite; break;
   case shared::plugin::yadomsApi::IYadomsApi::kReadOnly : keywordEntity.AccessMode = database::entities::kRead; break;
   case shared::plugin::yadomsApi::IYadomsApi::kWriteOnly: keywordEntity.AccessMode = database::entities::kWrite; break;
   default:
      BOOST_ASSERT_MSG(false, "Unknown accessMode");
      throw shared::exception::CEmptyResult("Fail to declare keyword : unknown accessMode");
      break;
   }
   keywordEntity.Name = keyword;
   keywordEntity.FriendlyName = keyword;
   keywordEntity.Details = details;

   m_keywordRequester->addKeyword(keywordEntity);

   return true;
}

bool CYadomsApiImplementation::capacityExists(const std::string& capacity) const
{
   // First look at standard capacities
   if (shared::plugin::yadomsApi::CStandardCapacities::Exists(capacity))
      return true;

   // Not found in standard capacities, look at user capacities
   //TODO en attendant que la table de UserCapacity existe
   return false;
//   return m_userCapacityRequester->capacityExists(capacity);
}

bool CYadomsApiImplementation::declareCapacity(const std::string& capacity)
{
   //TODO en attendant que la table de UserCapacity existe
   //if (capacityExists(capacity))
   //   return false;

   //m_userCapacityRequester->createCapacity(capacity);
   return true;
}
      
void CYadomsApiImplementation::historizeData(const std::string & device, const std::string & keyword, const std::string & value)
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

const shared::plugin::information::IInformation& CYadomsApiImplementation::getInformation() const
{
   return *m_informations;
}

const std::string CYadomsApiImplementation::getConfiguration() const
{
   return m_pluginData->Configuration;
}
        
bool CYadomsApiImplementation::recordPluginEvent(PluginEventSeverity severity, const std::string & message)
{
/*
   using the PluginLogEvent record a new log entry
*/
      
   //TODO : !
   return false;
}

shared::event::CEventHandler & CYadomsApiImplementation::getEventHandler()
{
   return m_pluginEventHandler;
}

int CYadomsApiImplementation::getPluginId() const
{
   return m_pluginData->Id;
}
	
} // namespace pluginSystem	
