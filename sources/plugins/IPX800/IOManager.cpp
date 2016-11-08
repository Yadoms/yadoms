#include "stdafx.h"
#include "IOManager.h"
#include "urlManager.h"
#include <boost/regex.hpp> 

CIOManager::CIOManager(const std::string& device, Poco::Net::IPAddress IPAddress, bool passwordActivated, std::string password):
     m_deviceName (device),
     m_IPAddress (IPAddress),
     m_isPasswordActivated(passwordActivated),
     m_password (password)
{}

void CIOManager::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, 
                            std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& RelayList,
                            std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& DIList,
                            std::vector<boost::shared_ptr<specificHistorizers::CAnalog> >& analogList,
                            std::vector<boost::shared_ptr<specificHistorizers::CCounter> >& counterList,
                            std::vector<boost::shared_ptr<extensions::IExtension> >& extensionList)
{
   m_RelayList = RelayList;
   m_DIList = DIList;
   m_analogList = analogList;
   m_countersList = counterList;
   m_devicesList = extensionList;
   m_keywordsToDeclare.clear();

   // Initial Reading of relays
   readIOFromDevice<specificHistorizers::CInputOuput, bool>(api, "R", m_RelayList);

   // Initial Reading of DIs
   readIOFromDevice<specificHistorizers::CInputOuput, bool>(api, "D", m_DIList);

   // Initial Reading of Analog Input
   readIOFromDevice<specificHistorizers::CAnalog, unsigned int>(api, "A", m_analogList);

   // Initial Reading of Counters
   readIOFromDevice<specificHistorizers::CCounter, Poco::Int64>(api, "C", m_countersList);

   //TODO : A réaliser dans une extension IPX800
   m_keywordsToDeclare.insert(m_keywordsToDeclare.end(), m_RelayList.begin(), m_RelayList.end());
   m_keywordsToDeclare.insert(m_keywordsToDeclare.end(), m_DIList.begin(), m_DIList.end());
   m_keywordsToDeclare.insert(m_keywordsToDeclare.end(), m_analogList.begin(), m_analogList.end());
   m_keywordsToDeclare.insert(m_keywordsToDeclare.end(), m_countersList.begin(), m_countersList.end());
   api->historizeData(m_deviceName, m_keywordsToDeclare);
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

template<typename T1, typename T2>
void CIOManager::readIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                  const std::string& type,
                                  const std::vector<boost::shared_ptr<T1> >& list)
{
   shared::CDataContainer parameters;
   shared::CDataContainer results;

   // add the password if activated
   if (m_isPasswordActivated)
      parameters.set("key", m_password);

   parameters.set("Get", type);

   results = urlManager::sendCommand(m_IPAddress, parameters);

   std::vector<boost::shared_ptr<T1> >::const_iterator diIterator;

   std::cout << "Nb IO : " << list.size() << std::endl;

   try
   {
      for (diIterator = list.begin(); diIterator != list.end(); ++diIterator)
      {
         std::cout << "Set IO : " << (*diIterator)->getHardwareName() << std::endl;
         T2 newValue = results.get<T2>((*diIterator)->getHardwareName());

         //historize only for new value
         if ((*diIterator)->get() != newValue)
         {
             (*diIterator)->set(newValue);
             m_keywordsToDeclare.push_back((*diIterator));
         }
      }

      std::vector<boost::shared_ptr<extensions::IExtension> >::const_iterator iteratorExtension;

      for (iteratorExtension = m_devicesList.begin(); iteratorExtension != m_devicesList.end(); ++iteratorExtension)
      {
         (*iteratorExtension)->updateFromDevice(api, results);
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "error retrieve value :" << e.what() << std::endl;
   }
}

CIOManager::~CIOManager()
{}