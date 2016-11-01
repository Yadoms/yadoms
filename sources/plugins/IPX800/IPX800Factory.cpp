#include "stdafx.h"
#include "IPX800Factory.h"
//#include "InitializationException.hpp"
//#include "eventDefinitions.h"

static const std::string Model("IPX800");

CIPX800Factory::CIPX800Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IIPX800Configuration& configuration,
                                 shared::CDataContainer details):
   m_ioManager(boost::make_shared<CIOManager>(device))
{
   // IO Configuration
/*
   for (int counter=0; counter<NB_OUTPUTS; ++counter)
   {
      std::string name = "DO" + boost::lexical_cast<std::string>(counter);
      m_DigitalOutput[counter] = boost::make_shared<CIO>( name, counter, EPullResistance::kDisable, yApi::EKeywordAccessMode::kGetSet);
      m_mapKeywordsName[ name ] = m_DigitalOutput[counter];
      m_keywordsToDeclare.push_back(m_DigitalOutput[counter]->historizable());
   }

   for (int counter=0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_DigitalInput[counter]  = boost::make_shared<CIO>( name, counter, configuration.PullResistanceState(counter), yApi::EKeywordAccessMode::kGet);
      m_mapKeywordsName[ name ] = m_DigitalInput[counter];
      m_keywordsToDeclare.push_back(m_DigitalInput[counter]->historizable());
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