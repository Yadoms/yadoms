#include "stdafx.h"
#include "IPX800Factory.h"
#include "equipments/IPX800Equipment.h"
#include "equipments/X8RExtension.h"
#include "equipments/X8DExtension.h"
#include "equipments/X24DExtension.h"
#include "equipments/manuallyDeviceCreationException.hpp"
#include <shared/Log.h>

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const IIPX800Configuration& configuration):
   m_ioManager(boost::make_shared<CIOManager>(device,
                                              configuration.getIPAddressWithSocket(),
                                              configuration.isPasswordActivated(),
                                              configuration.getPassword()))
{
   for (int counter = 0; counter < 6; ++counter)
   {
      X8RSlotused[counter] = false;
      X8DSlotused[counter] = false;
   }

   auto devices = api->getAllDevices();

   // Creation of the IPX800 module
   // This module is created by default
   boost::shared_ptr<equipments::IEquipment> extension = boost::make_shared<equipments::CIPX800Equipment>(api, device);
   m_devicesList.push_back(extension);
   //

   // Create all extensions devices
   for (std::vector<std::string>::iterator devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
      std::string type = "";
      // plugin state have no type
      try
      {
         type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");
      }
      catch (...)
      {
      }

      YADOMS_LOG(information) << "Name : " << (*devicesIterator) ;
      YADOMS_LOG(information) << "Model : " << type ;

      if (type == "X-8R")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         api->declareDevice(*devicesIterator, equipments::CX8RExtension::deviceType(), equipments::CX8RExtension::deviceType());
         extension = boost::make_shared<equipments::CX8RExtension>(api, *devicesIterator, position);
         m_devicesList.push_back(extension);
         X8RSlotused[position - 1] = true;
      }
      else if (type == "X-8D")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         api->declareDevice(*devicesIterator, equipments::CX8DExtension::deviceType(), equipments::CX8DExtension::deviceType());
         extension = boost::make_shared<equipments::CX8DExtension>(api, *devicesIterator, position);
         m_devicesList.push_back(extension);
         X8DSlotused[position - 1] = true;
      }
      else if (type == "X-24D")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         api->declareDevice(*devicesIterator, equipments::CX24DExtension::deviceType(), equipments::CX24DExtension::deviceType());
         extension = boost::make_shared<equipments::CX24DExtension>(api, *devicesIterator, position);
         m_devicesList.push_back(extension);

         // The corresponding slots for X-8D could not be installed
         X8DSlotused[position * 3] = true;
         X8DSlotused[position * 3 + 1] = true;
         X8DSlotused[position * 3 + 2] = true;
      }
   }

   m_ioManager->Initialize(m_devicesList);
}

void CIPX800Factory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const yApi::IManuallyDeviceCreationData& data)
{
   boost::shared_ptr<equipments::IEquipment> extension;

   try
   {
      if (data.getDeviceType() == "X8R")
      {
         int position = data.getConfiguration().get<int>("Position");
         extension = boost::make_shared<equipments::CX8RExtension>(api, data.getDeviceName(), position);
         X8RSlotused[position - 1] = true;
      }
      else if (data.getDeviceType() == "X8D")
      {
         int position = data.getConfiguration().get<int>("Position");
         extension = boost::make_shared<equipments::CX8DExtension>(api, data.getDeviceName(), position);
         X8DSlotused[position - 1] = true;
      }
      else if (data.getDeviceType() == "X24D")
      {
         int position = data.getConfiguration().get<int>("Position");
         extension = boost::make_shared<equipments::CX24DExtension>(api, data.getDeviceName(), position);

         // The corresponding slots for X-8D could not be installed
         X8DSlotused[(position - 1) * 3] = true;
         X8DSlotused[(position - 1) * 3 + 1] = true;
         X8DSlotused[(position - 1) * 3 + 2] = true;
      }

      if (extension)
         m_devicesList.push_back(extension);
   }
   catch (std::exception& e)
   {
      throw CManuallyDeviceCreationException(e.what());
   }

   // Update IO list
   m_ioManager->Initialize(m_devicesList);
}

shared::CDataContainer CIPX800Factory::bindSlotsX8R()
{
   shared::CDataContainer ev;
   std::string defaultValue = "";

   for (int counter = 0; counter < 6; ++counter)
   {
      // Add only not used slots
      if (!X8RSlotused[counter])
      {
         ev.set(boost::lexical_cast<std::string>(counter + 1), (boost::format("SLOT%1%") % (counter + 1)).str());
         if (defaultValue.empty())
            defaultValue = boost::lexical_cast<std::string>(counter + 1);
      }
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("name", "Slot");
   en.set("description", "Choose the slot");
   en.set("values", ev);
   en.set("defaultValue", defaultValue);

   return en;
}

shared::CDataContainer CIPX800Factory::bindSlotsX8D()
{
   shared::CDataContainer ev;
   std::string defaultValue = "";

   for (int counter = 0; counter < 6; ++counter)
   {
      // Add only not used slots
      if (!X8DSlotused[counter])
      {
         ev.set(boost::lexical_cast<std::string>(counter + 1), (boost::format("SLOT%1%") % (counter + 1)).str());

         if (defaultValue.empty())
            defaultValue = boost::lexical_cast<std::string>(counter + 1);
      }
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("name", "Slot");
   en.set("description", "Choose the slot");
   en.set("values", ev);
   en.set("defaultValue", defaultValue);

   return en;
}

shared::CDataContainer CIPX800Factory::bindSlotsX24D()
{
   shared::CDataContainer ev;
   std::string defaultValue = "";

   for (int counter = 0; counter < 2; ++counter)
   {
      // Add only not used slots
      if (!X8DSlotused[counter * 3] && !X8DSlotused[counter * 3 + 1] && !X8DSlotused[counter * 3 + 2])
      {
         ev.set(boost::lexical_cast<std::string>(counter + 1), (boost::format("SLOT%1%") % (counter + 1)).str());

         if (defaultValue.empty())
            defaultValue = boost::lexical_cast<std::string>(counter + 1);
      }
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("name", "Slot");
   en.set("description", "Choose the slot");
   en.set("values", ev);
   en.set("defaultValue", defaultValue);
   return en;
}

boost::shared_ptr<CIOManager> CIPX800Factory::getIOManager() const
{
   return m_ioManager;
}

void CIPX800Factory::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& deviceRemoved)
{
   for (unsigned char counter = 0; counter < m_devicesList.size(); ++counter)
   {
      YADOMS_LOG(information) << "device Name : " << m_devicesList[counter]->getDeviceName() ;
      YADOMS_LOG(information) << "deviceRemoved : " << deviceRemoved ;

      // Deletion from the list of the device
      if (m_devicesList[counter]->getDeviceName() == deviceRemoved)
      {
         // If it's an extension, we delete the extension.
         // If it's the IPX800, we delete all elements
         if (m_devicesList[counter]->getDeviceType() != "IPX800")
         {
            // free slot(s) associated to this device for future configurations
            const int position = m_devicesList[counter]->getSlot();

            if (m_devicesList[counter]->getDeviceType() == "X-8R") X8RSlotused[position - 1] = false;
            if (m_devicesList[counter]->getDeviceType() == "X-8D") X8DSlotused[position - 1] = false;
            if (m_devicesList[counter]->getDeviceType() == "X-24D")
            {
               X8DSlotused[(position - 1) * 3] = false;
               X8DSlotused[(position - 1) * 3 + 1] = false;
               X8DSlotused[(position - 1) * 3 + 2] = false;
            }

            // remove the extension
            m_devicesList.erase(m_devicesList.begin() + counter);
         }
         else // If IPX800, we delete all extensions
            m_devicesList.clear();
      }
   }
}

void CIPX800Factory::onDeviceConfigurationChange(const std::string& name,
                                                 const shared::CDataContainer& newConfiguration)
{
   for (std::vector<boost::shared_ptr<equipments::IEquipment>>::const_iterator iteratorExtension = m_devicesList.begin();
      iteratorExtension != m_devicesList.end();
      ++iteratorExtension)
   {
      if ((*iteratorExtension)->getDeviceName() == name)
      {
         YADOMS_LOG(trace) << "Configuration changed for device " << name;

         // free slot(s) associated to this device for future configurations
         auto position = (*iteratorExtension)->getSlot();
         const auto type = (*iteratorExtension)->getDeviceType();

         YADOMS_LOG(trace) << "free slot" << position;

         if (type == "X-8R") X8RSlotused[position - 1] = false;
         if (type == "X-8D") X8DSlotused[position - 1] = false;
         if (type == "X-24D")
         {
            X8DSlotused[(position - 1) * 3] = false;
            X8DSlotused[(position - 1) * 3 + 1] = false;
            X8DSlotused[(position - 1) * 3 + 2] = false;
         }

         (*iteratorExtension)->setNewConfiguration(newConfiguration);

         position = newConfiguration.get<int>("Position");

         if (type == "X-8R") X8RSlotused[position - 1] = true;
         if (type == "X-8D") X8DSlotused[position - 1] = true;
         if (type == "X-24D")
         {
            X8DSlotused[(position - 1) * 3] = true;
            X8DSlotused[(position - 1) * 3 + 1] = true;
            X8DSlotused[(position - 1) * 3 + 2] = true;
         }

         return;
      }
   }
}

CIPX800Factory::~CIPX800Factory()
{
}

