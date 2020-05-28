#include "stdafx.h"
#include "Piface2Factory.h"
#include "InitializationException.hpp"
#include "eventDefinitions.h"

#define NB_INPUTS  8
#define NB_OUTPUTS 8

static const std::string Model("Piface2");

CPiface2Factory::CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IPf2Configuration& configuration,
                                 boost::shared_ptr<shared::CDataContainer> details):
   m_ioManager(boost::make_shared<CIOManager>(device))
{
   // IO Configuration
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

   //Déclaration of all IOs
   api->declareDevice(device, Model, Model, m_keywordsToDeclare, details);

   m_ioManager->Initialize(api, m_mapKeywordsName);
}

boost::shared_ptr<CIOManager> CPiface2Factory::getIOManager(void)
{
   return m_ioManager;
}

void CPiface2Factory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const IPf2Configuration& configuration,
                                            boost::shared_ptr<shared::CDataContainer> details)
{
   for (int counter=0; counter<NB_INPUTS; ++counter)
      m_DigitalInput[counter]->ConfigurePullResistance( configuration.PullResistanceState(counter) );
}

CPiface2Factory::~CPiface2Factory()
{}
