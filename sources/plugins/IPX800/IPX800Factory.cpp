#include "stdafx.h"
#include "IPX800Factory.h"

static const std::string Model("IPX800");

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IIPX800Configuration& configuration,
                                 shared::CDataContainer details):
   m_ioManager(boost::make_shared<CIOManager>(device, configuration.getIPAddress(), configuration.isPasswordActivated(), configuration.getPassword()))
{
   /////////////////////////////////
   // Configuration de l'IPX800
   /////////////////////////////////

   // Relay Configuration
   for (int counter=0; counter<8; ++counter)
   {
      std::stringstream name;
      name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapDigitalInputOuput[ name.str() ] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

   // Analog Input Configuration
   for (int counter = 0; counter<4; ++counter)
   {
      std::stringstream name;
      name << "A" << std::setfill('0') << std::setw(1) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<specificHistorizers::CVirtualAnalog> temp = boost::make_shared<specificHistorizers::CVirtualAnalog>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapVirtualAnalogInput[name.str()] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

   // Counters Configuration
   for (int counter = 0; counter<16; ++counter)
   {
      std::stringstream name;
      name << "C" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapCounters[name.str()] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

   //Déclaration of all IOs
   api->declareDevice(device, Model, m_keywordsToDeclare, details);

   /////////////////////////////////
   // Configuration de l'IPX800 - Fin
   /////////////////////////////////

   m_ioManager->Initialize(api, m_mapDigitalInputOuput, m_mapVirtualAnalogInput, m_mapCounters);
}

boost::shared_ptr<CIOManager> CIPX800Factory::getIOManager(void)
{
   return m_ioManager;
}

void CIPX800Factory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const IIPX800Configuration& configuration,
                                            shared::CDataContainer details)
{
/*
   for (int counter=0; counter<NB_INPUTS; ++counter)
      m_DigitalInput[counter]->ConfigurePullResistance( configuration.PullResistanceState(counter) );
      */
}

CIPX800Factory::~CIPX800Factory()
{}