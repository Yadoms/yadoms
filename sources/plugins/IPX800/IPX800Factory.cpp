#include "stdafx.h"
#include "IPX800Factory.h"
#include "devices/X8RDevice.h"
#include "devices/X8DDevice.h"
#include "devices/X24DDevice.h"

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const IIPX800Configuration& configuration):
   m_ioManager(boost::make_shared<CIOManager>(device, configuration.getIPAddress(), configuration.isPasswordActivated(), configuration.getPassword()))
{
   createIPX800Device(api, device);

   m_ioManager->Initialize(api, m_mapDigitalInputOuput, m_mapVirtualAnalogInput, m_mapCounters);
}

void CIPX800Factory::createIPX800Device(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const std::string& device)
{
   const std::string Model("IPX800");

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

   shared::CDataContainer details;
   details.set("provider", "IPX800");
   details.set("shortProvider", "ipx");

   // Relay Configuration
   for (int counter = 0; counter<IPX800_RELAY_QTY; ++counter)
   {
      std::stringstream name;
      name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapDigitalInputOuput[name.str()] = temp;
      keywordsToDeclare.push_back(temp);
   }

   // Digital Input Configuration
   for (int counter = 0; counter<IPX800_DI_QTY; ++counter)
   {
      std::stringstream name;
      name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGet);
      m_mapDigitalInputOuput[name.str()] = temp;
      keywordsToDeclare.push_back(temp);
   }

   // Analog Input Configuration
   for (int counter = 0; counter<IPX800_ANA_QTY; ++counter)
   {
      std::stringstream name;
      name << "A" << std::setfill('0') << std::setw(1) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(name.str(), yApi::EKeywordAccessMode::kGet);
      m_mapVirtualAnalogInput[name.str()] = temp;
      keywordsToDeclare.push_back(temp);
   }

   // Counters Configuration
   for (int counter = 0; counter<IPX800_CNT_QTY; ++counter)
   {
      std::stringstream name;
      name << "C" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapCounters[name.str()] = temp;
      keywordsToDeclare.push_back(temp);
   }

   //Déclaration of all IOs
   api->declareDevice(device, Model, keywordsToDeclare, details);
}

std::string CIPX800Factory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                                 const yApi::IManuallyDeviceCreationData& data) const
{
   boost::shared_ptr<devices::IDevice> device;

   if (data.getConfiguration().get<bool>("type.content.X8R.radio"))
      device = boost::make_shared<devices::CX8RDevice>(api, data.getDeviceName());
   else if (data.getConfiguration().get<bool>("type.content.X8D.radio"))
      device = boost::make_shared<devices::CX8DDevice>(api, data.getDeviceName());
   else if (data.getConfiguration().get<bool>("type.content.X24D.radio"))
      device = boost::make_shared<devices::CX8DDevice>(api, data.getDeviceName());

   //TODO : A finir
   return data.getDeviceName();// device->getDeviceName();
}

void CIPX800Factory::createX8RDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const std::string& device)
{
   m_devicesList.push_back(boost::make_shared<devices::CX8RDevice>(api, device));
}


shared::CDataContainer CIPX800Factory::bindSlotsX8R()
{
   shared::CDataContainer ev;
   ev.set("SLOT1", "X8-R Slot 1");
   ev.set("SLOT2", "X8-R Slot 2");
   ev.set("SLOT3", "X8-R Slot 3");
   ev.set("SLOT4", "X8-R Slot 4");
   ev.set("SLOT5", "X8-R Slot 5");
   ev.set("SLOT6", "X8-R Slot 6");

   shared::CDataContainer en;
   en.set("type", "enum");
   en.set("values", ev);
   en.set("defaultValue", "SLOT1");

   return en;
}

void CIPX800Factory::createX8DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const std::string& device)
{
   m_devicesList.push_back(boost::make_shared<devices::CX8DDevice>(api, device));
}

void CIPX800Factory::createX24DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const std::string& device)
{
   m_devicesList.push_back(boost::make_shared<devices::CX24DDevice>(api, device));
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