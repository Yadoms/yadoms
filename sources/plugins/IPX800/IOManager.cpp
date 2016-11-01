#include "stdafx.h"
#include "IOManager.h"
//#include "IO.h"
#include "urlManager.h"
#include <boost/regex.hpp> 

CIOManager::CIOManager(const std::string& device, Poco::Net::IPAddress IPAddress, bool passwordActivated, std::string password):
     m_deviceName (device),
     m_IPAddress (IPAddress),
     m_isPasswordActivated(passwordActivated),
     m_password (password)
{}

void CIOManager::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                            std::map<std::string, 
                            boost::shared_ptr<yApi::historization::CSwitch> > IOlist)
{
   m_mapKeywordsName = IOlist;
   m_keywordsToDeclare.clear();

   //readIOFromDevice();

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
   //api->historizeData(m_deviceName, m_keywordsToDeclare);
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   shared::CDataContainer parameters;
   std::string keywordName = command->getKeyword();
   std::string commandSelected;

   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   auto search = m_mapKeywordsName.find(keywordName);

   if (search != m_mapKeywordsName.end())
   {
      // add the password if activated
      if (m_isPasswordActivated)
         parameters.set("key", m_password);

      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;

      //Set the digit number
      if (boost::regex_search(keywordName, match, reg))
      {
         // function selection + add the type of device
         switch (boost::lexical_cast<int>(command->getBody()))
         {
         case 0:
            commandSelected = "Clear" + match[1];
            break;
         case 1:
            commandSelected = "Set" + match[1];
            break;
         default:
            break;
         }

         parameters.set(commandSelected, match[2]);
      }

      if (urlManager::sendCommand(m_IPAddress, parameters))
      {
         // Set the value of the corresponding IO and historize if the return is success
         search->second->set(boost::lexical_cast<bool>(command->getBody()));
         api->historizeData(m_deviceName, search->second);
      }
   }
   else
      std::cerr << "Cannot find keyword : " << command->getKeyword();
}

void CIOManager::readIOFromDevice()
{
   shared::CDataContainer parameters;

   m_keywordsToDeclare.clear();

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   parameters.set("Get", "all");

   if (urlManager::sendCommand(m_IPAddress, parameters))
   {
      // Set the value of the corresponding IO and historize if the return is success
      //search->second->set(boost::lexical_cast<bool>(command->getBody()));
      //api->historizeData(m_deviceName, search->second);
   }
}

CIOManager::~CIOManager()
{}