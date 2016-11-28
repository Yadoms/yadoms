#include "stdafx.h"
#include "IPX800Equipment.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"

namespace equipments
{
   CIPX800Equipment::CIPX800Equipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const std::string& device):
      m_deviceName(device),
      m_deviceType("IPX800")
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      shared::CDataContainer details;
      details.set("provider", "IPX800");
      details.set("shortProvider", "ipx");
      details.set("type", m_deviceType);

      // Relay Configuration
      for (int counter = 0; counter<IPX800_RELAY_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "R" << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Digital Input Configuration
      for (int counter = 0; counter<IPX800_DI_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "D" << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGet);
         m_DIList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Analog Input Configuration
      for (int counter = 0; counter<IPX800_ANA_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "A" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "A" << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(name.str(),
                                                                                                                 hardwareName.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_analogList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Counters Configuration
      for (int counter = 0; counter<IPX800_CNT_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "C" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "C" << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<specificHistorizers::CCounter> temp = boost::make_shared<specificHistorizers::CCounter>(name.str(),
                                                                                                                   hardwareName.str(),
                                                                                                                   yApi::EKeywordAccessMode::kGetSet);
         m_countersList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CIPX800Equipment::getDeviceName() const
   {
      return m_deviceName;
   }

   std::string CIPX800Equipment::getDeviceType() const
   {
      return m_deviceType;
   }

   int CIPX800Equipment::getSlot() const
   {
      throw shared::exception::CException("Module IPX800 have no slot");
   }

   void CIPX800Equipment::updateFromDevice(const std::string& type,
                                           boost::shared_ptr<yApi::IYPluginApi> api,
                                           shared::CDataContainer& values,
                                           bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

      if (type == "R") updateIOFromDevice<specificHistorizers::CInputOuput, bool>(api, values, m_relaysList, keywordsToHistorize, forceHistorization);
      if (type == "D") updateIOFromDevice<specificHistorizers::CInputOuput, bool>(api, values, m_DIList, keywordsToHistorize, forceHistorization);
      if (type == "A") updateIOFromDevice<specificHistorizers::CAnalog, unsigned int>(api, values, m_analogList, keywordsToHistorize, forceHistorization);
      if (type == "C") updateIOFromDevice<specificHistorizers::CCounter, Poco::Int64>(api, values, m_countersList, keywordsToHistorize, forceHistorization);

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   template<class T1, class T2>
   void CIPX800Equipment::updateIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                             shared::CDataContainer& values,
                                             std::vector<boost::shared_ptr<T1> >& keywordsList,
                                             std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& ToHistorize,
                                             bool forceHistorization)
   {
      typename std::vector<boost::shared_ptr<T1> >::const_iterator diIterator;
      std::string productRevision("");

      try
      {
         productRevision = values.getWithDefault<std::string>("product","");
         for (diIterator = keywordsList.begin(); diIterator != keywordsList.end(); ++diIterator)
         {
            T2 newValue = values.get<T2>((*diIterator)->getHardwareName());

            //historize only for new value
            if ((*diIterator)->get() != newValue || forceHistorization)
            {
               std::cout << "read IO : " << (*diIterator)->getHardwareName() << " : " << boost::lexical_cast<std::string>(newValue) << std::endl;
               (*diIterator)->set(newValue);
               ToHistorize.push_back((*diIterator));
            }
         }
      }
      catch (shared::exception::CInvalidParameter&)
      {
         // If we could read the product revision, IOs have not been return : so it's an error of conception, or a wrong apikey
         if (productRevision == "IPX800_V4") throw CNoInformationException("Equipment connected, but no information received. Please check apikey");
      }
   }

   void CIPX800Equipment::historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      if (m_pendingIOHistorizer)
      {
         bool newValue = false; 
         if (command->getBody() == "true")
            newValue = true;
         else if (command->getBody() == "false")
            newValue = false;
         else
            shared::exception::CException("Invalid command");

         if (m_pendingIOHistorizer->get() != newValue)
         {
            m_pendingIOHistorizer->set(newValue);
            api->historizeData(m_deviceName, m_pendingIOHistorizer);
         }

         m_pendingIOHistorizer.reset();
      }

      if (m_pendingCounterHistorizer)
      {
         Poco::Int64 newValue = boost::lexical_cast<Poco::Int64>(command->getBody());
         if (m_pendingCounterHistorizer->get() != newValue)
         {
            m_pendingCounterHistorizer->set(newValue);
            api->historizeData(m_deviceName, m_pendingCounterHistorizer);
         }

         m_pendingCounterHistorizer.reset();
      }
   }

   void CIPX800Equipment::resetPendingCommand()
   {
      m_pendingIOHistorizer.reset();
      m_pendingCounterHistorizer.reset();
   }

   shared::CDataContainer CIPX800Equipment::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                                                 shared::CDataContainer& parameters, 
                                                                 boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::string keywordName = command->getKeyword();

      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;

      std::cout << "CIPX800Equipment::buildMessageToDevice" << std::endl;

      //Set parameters
      if (boost::regex_search(keywordName, match, reg))
      {
         if (match[1] == "R") setParameter(keywordName, m_relaysList, command, match[2], parameters);
         else if (match[1] == "C") setParameter(keywordName, m_countersList, command, command->getBody(), parameters);
         else throw shared::exception::CException("Invalid or Read-Only Keyword:" + keywordName);

         // No command should be received for DI and Analog Values
      }

      return parameters;
   }

   void CIPX800Equipment::setParameter(const std::string& keywordName, 
                                       std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >& keywordsList,
                                       boost::shared_ptr<const yApi::IDeviceCommand> command,
                                       const std::string& pinNumber,
                                       shared::CDataContainer& parameters)
   {
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >::const_iterator diIterator;

      std::cout << "keywordName" << keywordName <<std::endl;

      for (diIterator = keywordsList.begin(); diIterator != keywordsList.end(); ++diIterator)
      {
         // Keyword found
         if ((*diIterator)->getKeyword() == keywordName)
         {
            bool newValue = false;

            if (command->getBody() == "true")
               newValue = true;
            else if (command->getBody() == "false")
               newValue = false;
            else
               shared::exception::CException("Invalid command");

            if (newValue)
               parameters.set("SetR", pinNumber); 
            else
               parameters.set("ClearR", pinNumber);

            m_pendingIOHistorizer = (*diIterator);
         }
      }

      //return parameters;
   }

   void CIPX800Equipment::setParameter(const std::string& keywordName,
                                       std::vector<boost::shared_ptr<specificHistorizers::CCounter> >& keywordsList,
                                       boost::shared_ptr<const yApi::IDeviceCommand> command,
                                       const std::string& value,
                                       shared::CDataContainer& parameters)
   {
      std::vector<boost::shared_ptr<specificHistorizers::CCounter> >::const_iterator diIterator;

      std::cout << "CIPX800Equipment::setParameter counter" << std::endl;

      for (diIterator = keywordsList.begin(); diIterator != keywordsList.end(); ++diIterator)
      {
         // Keyword found
         if ((*diIterator)->getKeyword() == keywordName)
         {
            if (boost::lexical_cast<int>(value) == 0)
            {
               // Not HardwareName ! 
               // It's an exception for the IPX800, to set to 0, we have to enter SetC02=0 (for example !), the format is different from the json reading "C2=0"

               parameters.set("Set" + (*diIterator)->getKeyword(), value);
               m_pendingCounterHistorizer = (*diIterator);
            }
            else
               std::cerr << (*diIterator)->getKeyword() << " could not be set to a value. Only reset is possible." << std::endl;
         }
         //0   : counter is cleared -> Only this one is implemented
         //!=0 : the value is added or sub from the counter
      }
   }

   CIPX800Equipment::~CIPX800Equipment()
   {}
}// namespace equipments