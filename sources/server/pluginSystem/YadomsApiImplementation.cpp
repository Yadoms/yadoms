#include "stdafx.h"
#include "YadomsApiImplementation.h"
#include <shared/FileSystemExtension.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>

namespace pluginSystem
{

CYadomsApiImplementation::CYadomsApiImplementation(const boost::shared_ptr<database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IDeviceRequester> deviceRequester)
   :m_informations(shared::CFileSystemExtension::getModulePath()), m_pluginData(pluginData), m_deviceRequester(deviceRequester)
{
}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string & deviceName)
{
   return !!m_deviceRequester->getDevice(getPluginId(), deviceName);
}

bool CYadomsApiImplementation::declareNewDevice(const std::string & deviceName, const std::vector<shared::plugin::yadomsApi::CCapacity> & capacities)
{
   if (deviceExists(deviceName))
      throw shared::exception::CInvalidParameter(deviceName);

   try
   {
      m_deviceRequester->createDevice(getPluginId(), deviceName, deviceName);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Fail to create " << deviceName << " device : " << e.what();
      return false;
   }

   return true;
}
      
void CYadomsApiImplementation::historizeData(const std::string & deviceName, const std::string & keyword, const shared::plugin::yadomsApi::CCapacity & capacity, const std::string & value)
{
/*
   using the database requester save new capacity value
*/
      
   //TODO : !
}

const shared::plugin::information::IInformation& CYadomsApiImplementation::getInformation() const
{
   return m_informations;
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
