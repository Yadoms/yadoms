#include "stdafx.h"
#include "Piface2Factory.h"
#include "wiringPi.h"
#include "InitializationException.hpp"
#include <errno.h>

#define NB_INPUTS  8
#define NB_OUTPUTS 8

static const std::string Model("Piface2");

shared::event::CEventHandler CPiface2Factory::m_Event;

CPiface2Factory::CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 const IPf2Configuration& configuration,
                                 shared::CDataContainer details,
                                 int forId)
{
   // Initializing wiringPi in Gpio mode
   // need root privilege, but needed of pullup or pulldown
   if (wiringPiSetupGpio() ==-1 )
      throw CInitializationException( strerror (errno) );

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
      m_DigitalInput[counter]  = boost::make_shared<CIO>( name , counter, configuration.PullResistanceState(counter), yApi::EKeywordAccessMode::kGet);
      m_mapKeywordsName[ name ] = m_DigitalOutput[counter];
      m_keywordsToDeclare.push_back(m_DigitalInput[counter]->historizable());
   }

   //Déclaration of all IOs
   api->declareDevice(device, Model, m_keywordsToDeclare, details);
}

void CPiface2Factory::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                boost::shared_ptr<const yApi::IDeviceCommand> command,
                                bool fromInput)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(command->getKeyword());

   if (search != m_mapKeywordsName.end())
      search->second->set(boost::lexical_cast<bool>(command->getBody()), !fromInput);
   else
      std::cerr << "Cannot find keyword " << command->getKeyword();
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

CPiface2Factory::~CPiface2Factory()
{
}
