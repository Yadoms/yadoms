#include "stdafx.h"
#include "WESFactory.h"
#include "equipments/WESEquipment.h"
#include "equipments/manuallyDeviceCreationException.hpp"
#include <shared/Log.h>

CWESFactory::CWESFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                         boost::shared_ptr<IWESConfiguration> configuration)
{

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   // This module is created by default
   //boost::shared_ptr<equipments::IEquipment> extension;

   // TODO : To be done 
   // Creation of the WES module
   //extension = boost::make_shared<equipments::CWESEquipment>(api);
   //m_devicesList.push_back(extension);
   //

   // Create all devices and equipments
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
	   std::string type = "";
      // plugin state have no type
	   try {
		   type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");
	   }
	   catch (...)
	   {}

      YADOMS_LOG(information) << "Name : " << (*devicesIterator) ;
      YADOMS_LOG(information) << "Model : " << type ;
   }
}

std::string CWESFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                              const yApi::IManuallyDeviceCreationData& data)
{
   boost::shared_ptr<equipments::IEquipment> extension;

   try {

      // Faire la création du bon ID

      if (extension)
         m_devicesList.push_back(extension);
   }
   catch (std::exception& e)
   {
      throw CManuallyDeviceCreationException(e.what());
   }

   return extension->getDeviceName();
}

void CWESFactory::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_devicesList.size(); ++counter)
   {
      YADOMS_LOG(information) << "device Name : " << m_devicesList[counter]->getDeviceName() ;
      YADOMS_LOG(information) << "deviceRemoved : " << deviceRemoved ;

      // Deletion from the list of the device
      if (m_devicesList[counter]->getDeviceName() == deviceRemoved)
      {

         // If it's an extension, we delete the extension.
         // If it's the WES
         //if (m_devicesList[counter]->getDeviceType() != "WES")
         //{
            // free slot(s) associated to this device for future configurations
            //int position = m_devicesList[counter]->getSlot();
            
            // remove the extension
            m_devicesList.erase(m_devicesList.begin() + counter);
         //}
      }
   }
}

CWESFactory::~CWESFactory()
{}