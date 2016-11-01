#include "stdafx.h"
#include "IOManager.h"
#include "IO.h"

CIOManager::CIOManager(const std::string& device):
     m_deviceName (device)
{}

void CIOManager::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                            std::map<std::string, boost::shared_ptr<IIO> > IOlist)
{
   m_mapKeywordsName = IOlist;
   m_keywordsToDeclare.clear();

   // Initial reading of DI
/*
   for (int counter = 0; counter<NB_INPUTS; ++counter)
   {
      std::string name = "DI" + boost::lexical_cast<std::string>(counter);
      m_inputValue |= (m_mapKeywordsName[name]->get() << counter);
      m_keywordsToDeclare.push_back(m_mapKeywordsName[name]->historizable());
   }
   */
   //Historize initial values
   api->historizeData(m_deviceName, m_keywordsToDeclare);
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(command->getKeyword());

   if (search != m_mapKeywordsName.end())
   {
      // Set the value of the corresponding IO and historize 
      search->second->set(boost::lexical_cast<bool>(command->getBody()));
      api->historizeData(m_deviceName, search->second->historizable());
   }
   else
      std::cerr << "Cannot find keyword : " << command->getKeyword();
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           int receivedValue)
{
   m_keywordsToDeclare.clear();

   std::cout << "Value received :" << std::hex << receivedValue << std::endl;

   api->historizeData(m_deviceName, m_keywordsToDeclare);
}

CIOManager::~CIOManager()
{}