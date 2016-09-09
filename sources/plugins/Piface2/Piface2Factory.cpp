#include "stdafx.h"
#include "Piface2Factory.h"
#include "wiringPi.h"

static const std::string Model("Piface2");

CPiface2Factory::CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 shared::CDataContainer details,
                                 int forId)
{
   // Initializing wiringPi
   wiringPiSetup();

   // IO Configuration
   m_WriteIO = boost::make_shared<CIO>("IO1", 0, yApi::EKeywordAccessMode::kGetSet);
   m_ReadIO  = boost::make_shared<CIO>("IO2", 1, yApi::EKeywordAccessMode::kGet);

   m_mapKeywordsName["IO1"] = m_WriteIO;
   m_mapKeywordsName["IO2"] = m_ReadIO;

   // IO subscription
   m_ReadIO->subscribeForConnectionEvents(m_Event, forId);

   //Déclaration of all IOs
   m_keywordsToDeclare.push_back(m_WriteIO->historizable());
   m_keywordsToDeclare.push_back(m_ReadIO->historizable());
   api->declareDevice(device, Model, m_keywordsToDeclare, details);
}

void CPiface2Factory::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(command->getKeyword());

   if (search != m_mapKeywordsName.end())
      search->second->write(boost::lexical_cast<bool>(command->getBody()));
   else
      std::cerr << "Cannot find keyword " << command->getKeyword();
}

CPiface2Factory::~CPiface2Factory()
{
}
