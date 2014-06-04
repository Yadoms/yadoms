#include "stdafx.h"
#include "YadomsApiImplementation.h"
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

bool CYadomsApiImplementation::declareDevice(const std::string& device, const std::string& model)
{
   if (deviceExists(device))
      return false;

   m_deviceRequester->createDevice(getPluginId(), device, device, model);
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
