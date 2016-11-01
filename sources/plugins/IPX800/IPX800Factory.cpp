#include "stdafx.h"
#include "IPX800Factory.h"

static const std::string Model("IPX800");

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IIPX800Configuration& configuration,
                                 shared::CDataContainer details):
   m_ioManager(boost::make_shared<CIOManager>(device, configuration.GetIPAddress(), configuration.isPasswordActivated(), configuration.GetPassword()))
{
   // Relay Configuration
   for (int counter=0; counter<8; ++counter)
   {
      std::stringstream name;
      name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapKeywordsName[ name.str() ] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

   // Virtual Output Configuration
   for (int counter = 0; counter<8; ++counter)
   {
      std::stringstream name;
      name << "VO" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGetSet);
      m_mapKeywordsName[name.str()] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

   // Virtual Input Configuration
   for (int counter = 0; counter<8; ++counter)
   {
      std::stringstream name;
      name << "VI" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
      boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGet);
      m_mapKeywordsName[name.str()] = temp;
      m_keywordsToDeclare.push_back(temp);
   }

/*
   // Analog Input Configuration
   for (int counter = 0; counter<4; ++counter)
   {
      std::string name = "VA" + boost::lexical_cast<std::string>(counter + 1);
      boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(name, yApi::EKeywordAccessMode::kGet);
      m_mapKeywordsName[name] = temp;
      m_keywordsToDeclare.push_back(temp);
   }
   */
   //Déclaration of all IOs
   api->declareDevice(device, Model, m_keywordsToDeclare, details);

   m_ioManager->Initialize(api, m_mapKeywordsName);
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