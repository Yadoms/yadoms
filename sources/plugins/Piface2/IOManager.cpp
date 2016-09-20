#include "stdafx.h"
#include "IOManager.h"
#include "InitializationException.hpp"
#include <errno.h>

#define NB_INPUTS  8
#define NB_OUTPUTS 8

static const std::string Model("Piface2");

CIOManager::CIOManager(const std::string& device)
   : m_deviceName (device)
{}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
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

void CIOManager::setNewIOList(std::map<std::string, boost::shared_ptr<CIO> > IOlist)
{
   m_mapKeywordsName = IOlist;
}

CIOManager::~CIOManager()
{}