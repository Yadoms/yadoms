#include "stdafx.h"
#include "Piface2Factory.h"

static const std::string Model("Piface2");

CPiface2Factory::CPiface2Factory(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string& device,
                                 shared::CDataContainer details,
                                 int forId)
{
   // TODO : initialisation WiringPi

   // IO Configuration
   m_WriteIO = boost::make_shared<CIO>("IO1", 0, EIOType::kWrite);
   m_ReadIO  = boost::make_shared<CIO>("IO2", 0, EIOType::kRead);

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

   // TODO : Dispatch entre les IOs, et write ce qu'il va bien
}

CPiface2Factory::~CPiface2Factory()
{
}