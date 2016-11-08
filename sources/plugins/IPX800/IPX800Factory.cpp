#include "stdafx.h"
#include "IPX800Factory.h"
#include "extensions/X8RExtension.h"
#include "extensions/X8DExtension.h"
#include "extensions/X24DExtension.h"

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const IIPX800Configuration& configuration):
   m_ioManager(boost::make_shared<CIOManager>(device, configuration.getIPAddress(), configuration.isPasswordActivated(), configuration.getPassword()))
{
   for (int counter = 0; counter < 6; ++counter)
   {
      X8RSlotused[counter] = false;
      X8DSlotused[counter] = false;
   }

   for (int counter = 0; counter < 2; ++counter)
      X24DSlotused[counter] = false;

   // Creation of the IPX800 device
   createIPX800Device(api, device);

   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   // Create al the other devices
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
      boost::shared_ptr<extensions::IExtension> extension;
      std::string model = api->getDeviceModel(*devicesIterator);
      std::cout << "Name : " << (*devicesIterator) << std::endl;
      std::cout << "Model : " << model << std::endl;

      if (model == "X-8R")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         std::string deviceName = details.get<std::string>("provider");
         X8RSlotused[position] = true;
         extension = boost::make_shared<extensions::CX8RExtension>(api, deviceName, position);
      }
      else if (model == "X-8D")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         std::string deviceName = details.get<std::string>("provider");
         X8DSlotused[position] = true;
         extension = boost::make_shared<extensions::CX8DExtension>(api, deviceName, position);
      }
      else if (model == "X-24D")
      {
         auto details = api->getDeviceDetails((*devicesIterator));
         int position = details.get<int>("position");
         std::string deviceName = details.get<std::string>("provider");
         X24DSlotused[position] = true;
         extension = boost::make_shared<extensions::CX24DExtension>(api, deviceName, position);
      }

      m_devicesList.push_back(extension);
   }

   m_ioManager->Initialize(api, m_relaysList, m_DIList, m_analogList, m_countersList, m_devicesList);
}

void CIPX800Factory::createIPX800Device(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const std::string& device)
{
   const std::string Model("IPX800");

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

   shared::CDataContainer details;
   details.set("provider", "IPX800");
   details.set("shortProvider", "ipx");
   details.set("type", "IPX800");

   // Relay Configuration
   for (int counter = 0; counter<IPX800_RELAY_QTY; ++counter)
   {
      std::stringstream name, hardwareName;
      name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      hardwareName << "R" << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(), 
                                                                                                                      hardwareName.str(),
                                                                                                                      yApi::EKeywordAccessMode::kGetSet);
      m_relaysList.push_back (temp);
      keywordsToDeclare.push_back(temp);
   }

   // Digital Input Configuration
   for (int counter = 0; counter<IPX800_DI_QTY; ++counter)
   {
      std::stringstream name, hardwareName;
      name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      hardwareName << "D" << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(), 
                                                                                                                      hardwareName.str(),
                                                                                                                      yApi::EKeywordAccessMode::kGet);
      m_DIList.push_back(temp);
      keywordsToDeclare.push_back(temp);
   }

   // Analog Input Configuration
   for (int counter = 0; counter<IPX800_ANA_QTY; ++counter)
   {
      std::stringstream name, hardwareName;
      name << "A" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      hardwareName << "A" << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(name.str(), 
                                                                                                              hardwareName.str(),
                                                                                                              yApi::EKeywordAccessMode::kGet);
      m_analogList.push_back(temp);
      keywordsToDeclare.push_back(temp);
   }

   // Counters Configuration
   for (int counter = 0; counter<IPX800_CNT_QTY; ++counter)
   {
      std::stringstream name, hardwareName;
      name << "C" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      hardwareName << "C" << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CCounter> temp = boost::make_shared<specificHistorizers::CCounter>(name.str(),
                                                                                                                hardwareName.str(),
                                                                                                                yApi::EKeywordAccessMode::kGetSet);
      m_countersList.push_back(temp);
      keywordsToDeclare.push_back(temp);
   }

   //Déclaration of all IOs
   api->declareDevice(device, Model, keywordsToDeclare, details);
}

std::string CIPX800Factory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                                 const yApi::IManuallyDeviceCreationData& data)
{
   boost::shared_ptr<extensions::IExtension> extension;

   if (data.getConfiguration().get<bool>("type.content.X8R.radio"))
   {
      int position = data.getConfiguration().get<int>("type.content.X8R.content.Position");
      extension = boost::make_shared<extensions::CX8RExtension>(api, data.getDeviceName(), position);
      X8RSlotused[position] = true;
   }
   else if (data.getConfiguration().get<bool>("type.content.X8D.radio"))
   {
      int position = data.getConfiguration().get<int>("type.content.X8D.content.Position");
      extension = boost::make_shared<extensions::CX8DExtension>(api, data.getDeviceName(), position);
      X8DSlotused[position] = true;
   }
   else if (data.getConfiguration().get<bool>("type.content.X24D.radio"))
   {
      int position = data.getConfiguration().get<int>("type.content.X24D.content.Position");
      extension = boost::make_shared<extensions::CX24DExtension>(api, data.getDeviceName(), position);
      X24DSlotused[position] = true;
   }

   m_devicesList.push_back(extension);

   return extension->getDeviceName();
}

shared::CDataContainer CIPX800Factory::bindSlotsX8R()
{
   shared::CDataContainer ev;

   for (int counter = 0; counter < 6; ++counter)
   {
      // Add only not used slots
      if (!X8RSlotused[counter])
         ev.set(boost::lexical_cast<std::string>(counter + 1), "");
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "1");

   return en;
}

shared::CDataContainer CIPX800Factory::bindSlotsX8D()
{
   shared::CDataContainer ev;

   for (int counter = 0; counter < 6; ++counter)
   {
      // Add only not used slots
      if (!X8DSlotused[counter])
         ev.set(boost::lexical_cast<std::string>(counter), "");
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "SLOT1");

   return en;
}

shared::CDataContainer CIPX800Factory::bindSlotsX24D()
{
   shared::CDataContainer ev;

   for (int counter = 0; counter < 2; ++counter)
   {
      // Add only not used slots
      if (!X24DSlotused[counter])
         ev.set(boost::lexical_cast<std::string>(counter), "");
   }

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "SLOT1");

   return en;
}

boost::shared_ptr<CIOManager> CIPX800Factory::getIOManager(void)
{
   return m_ioManager;
}

void CIPX800Factory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const IIPX800Configuration& configuration)
{
/*
   for (int counter=0; counter<NB_INPUTS; ++counter)
      m_DigitalInput[counter]->ConfigurePullResistance( configuration.PullResistanceState(counter) );
      */
}

CIPX800Factory::~CIPX800Factory()
{}