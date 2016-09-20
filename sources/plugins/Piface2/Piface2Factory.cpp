#include "stdafx.h"
#include "Piface2Factory.h"
#include "pifacedigital.h"
#include "InitializationException.hpp"
#include <errno.h>

#define NB_INPUTS  8
#define NB_OUTPUTS 8

static const std::string Model("Piface2");

CPiface2Factory::CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IPf2Configuration& configuration,
                                 shared::CDataContainer details):
   m_ioManager(boost::make_shared<CIOManager>(device))
{
   // Open the connection
   if (pifacedigital_open( 0 ) == -1)
      throw CInitializationException("Initialization error - Configuration of the SPI in raspi-config ?");

   // IO Configuration
   for (int counter=0; counter<NB_OUTPUTS; ++counter)
   {
      std::string name = "DO" + boost::lexical_cast<std::string>(counter);
      m_DigitalOutput[counter] = boost::make_shared<CIO>( name, counter, EPullResistance::kDisable, yApi::EKeywordAccessMode::kGetSet, m_InterruptHandler);
      m_mapKeywordsName[ name ] = m_DigitalOutput[counter];
      m_keywordsToDeclare.push_back(m_DigitalOutput[counter]->historizable());
   }

   for (int counter=0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_DigitalInput[counter]  = boost::make_shared<CIO>( name, counter, configuration.PullResistanceState(counter), yApi::EKeywordAccessMode::kGet, m_InterruptHandler);
      m_mapKeywordsName[ name ] = m_DigitalOutput[counter];
      m_keywordsToDeclare.push_back(m_DigitalInput[counter]->historizable());
   }

   // Copy all IOs created to the IO Manager
   m_ioManager->setNewIOList(getAllDigitalIO());

   //Déclaration of all IOs
   api->declareDevice(device, Model, m_keywordsToDeclare, details);
}

boost::shared_ptr<CIOManager> CPiface2Factory::getIOManager(void)
{
   return m_ioManager;
}

void CPiface2Factory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const IPf2Configuration& configuration,
                                            shared::CDataContainer details)
{
   for (int counter=0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_DigitalInput[counter]->ConfigurePullResistance( configuration.PullResistanceState(counter) );
   }
}

std::map<std::string, boost::shared_ptr<CIO> > CPiface2Factory::getAllDigitalIO(void)
{
   return m_mapKeywordsName;
}

CPiface2Factory::~CPiface2Factory()
{
   // Close de connection
   pifacedigital_close(0);
}