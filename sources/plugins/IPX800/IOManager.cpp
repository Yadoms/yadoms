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
                            std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > RelayList,
                            std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> > DIList,
                            std::vector<boost::shared_ptr<specificHistorizers::CAnalog> > analogList,
                            std::vector<boost::shared_ptr<yApi::historization::CCounter> > counterList)
{
   m_RelayList = RelayList;
   m_DIList = DIList;
   m_analogList = analogList;
   m_countersList = counterList;
   m_keywordsToDeclare.clear();

   readIOFromDevice();
}

void CIOManager::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   shared::CDataContainer parameters;
   std::string keywordName = command->getKeyword();
   std::string commandSelected;

   /*

   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   auto search = m_mapDigitalInputOuput.find(keywordName);

   // is it found in the IOList
   if (search != m_mapDigitalInputOuput.end())
   {
      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;

      //Set the digit number
      if (boost::regex_search(keywordName, match, reg))
      {
         // In case of keywords that begins by 'R', 'VI', or 'VO'
         if (match[1].compare("R") == 0 || match[1].compare("VI") == 0 || match[1].compare("VO") == 0)
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
      }

      if (urlManager::sendCommand(m_IPAddress, parameters))
      {
         // Set the value of the corresponding IO and historize if the return is success
         search->second->set(boost::lexical_cast<bool>(command->getBody()));
         api->historizeData(m_deviceName, search->second);
      }
   }

   auto search1 = m_mapCounters.find(keywordName);

   // is it found in the IOList
   if (search1 != m_mapCounters.end())
   {
      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;

      //Set the digit number
      if (boost::regex_search(keywordName, match, reg))
      {
         // In case of keywords that begins by 'C' -> Counters
         // 'Set' have a different behaviour that for 
         if (match[1].compare("C") == 0)
         {
            commandSelected = "Set" + match[1];
            parameters.set(commandSelected, match[2]);
         }
      }

      if (urlManager::sendCommand(m_IPAddress, parameters))
      {
         // Set the value of the corresponding IO and historize if the return is success
         search->second->set(boost::lexical_cast<bool>(command->getBody()));
         api->historizeData(m_deviceName, search->second);
      }
   }
   */
   
   //std::cerr << "Cannot find keyword : " << command->getKeyword();
}

void CIOManager::readIOFromDevice()
{
   shared::CDataContainer parameters;
   shared::CDataContainer results;

   m_keywordsToDeclare.clear();

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   parameters.set("Get", "R");

   results = urlManager::sendCommand(m_IPAddress, parameters);

   parameters.printToLog();
   results.printToLog();

   std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >::iterator relayIterator;

   std::cout << "Nb Relays : " << m_RelayList.size() << std::endl;

   try
   {
      for (relayIterator = m_RelayList.begin(); relayIterator != m_RelayList.end(); ++relayIterator)
      {
         (*relayIterator)->set(results.get<bool>((*relayIterator)->getHardwareName()));
         //std::cout << "Hardware name : " << (*relayIterator)->getHardwareName() << std::endl;
      }
   }
   catch (...)
   {
      std::cout << "error retrieve relais" << std::endl;
   }

   //{
      // Set the value of the corresponding IO and historize if the return is success
      //search->second->set(boost::lexical_cast<bool>(command->getBody()));
      //api->historizeData(m_deviceName, search->second);
   //}
   /*
   parameters.set("Get", "D");

   results = urlManager::sendCommand(m_IPAddress, parameters);
   //{
      // Set the value of the corresponding IO and historize if the return is success
      //search->second->set(boost::lexical_cast<bool>(command->getBody()));
      //api->historizeData(m_deviceName, search->second);
   //}

   parameters.set("Get", "A");

   results = urlManager::sendCommand(m_IPAddress, parameters);
   //{
      // Set the value of the corresponding IO and historize if the return is success
      //search->second->set(boost::lexical_cast<bool>(command->getBody()));
      //api->historizeData(m_deviceName, search->second);
   //}

   parameters.set("Get", "C");

   results = urlManager::sendCommand(m_IPAddress, parameters);
   //{
      // Set the value of the corresponding IO and historize if the return is success
      //search->second->set(boost::lexical_cast<bool>(command->getBody()));
      //api->historizeData(m_deviceName, search->second);
   //}
   */
}

CIOManager::~CIOManager()
{}