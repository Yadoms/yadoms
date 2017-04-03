#include "stdafx.h"
#include "WESEquipment.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include <shared/Log.h>

namespace equipments
{
   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const std::string& device):
      m_deviceName(device),
      m_deviceType("WES")
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      shared::CDataContainer details;
      details.set("provider", "WES");
      details.set("shortProvider", "ipx");
      details.set("type", m_deviceType);

      // TODO : Declarations of all IOs here ...

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CWESEquipment::getDeviceName() const
   {
      return m_deviceName;
   }

   std::string CWESEquipment::getDeviceType() const
   {
      return m_deviceType;
   }

   int CWESEquipment::getSlot() const
   {
      throw shared::exception::CException("Module WES have no slot");
   }

   void CWESEquipment::updateFromDevice(const std::string& type,
                                           boost::shared_ptr<yApi::IYPluginApi> api,
                                           shared::CDataContainer& values,
                                           bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   template<class T1, class T2>
   void CWESEquipment::updateIOFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
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
               YADOMS_LOG(information) << "read IO : " << (*diIterator)->getHardwareName() << " : " << boost::lexical_cast<std::string>(newValue) ;
               (*diIterator)->set(newValue);
               ToHistorize.push_back((*diIterator));
            }
         }
      }
      catch (shared::exception::CInvalidParameter&)
      {
         // If we could read the product revision, IOs have not been return : so it's an error of conception, or a wrong apikey
         if (productRevision == "WES_V4") throw CNoInformationException("Equipment connected, but no information received. Please check apikey");
      }
   }

   void CWESEquipment::historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
   }

   void CWESEquipment::resetPendingCommand()
   {
   }

   shared::CDataContainer CWESEquipment::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                                                 shared::CDataContainer& parameters, 
                                                                 boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::string keywordName = command->getKeyword();

      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;
/*
      //Set parameters
      if (boost::regex_search(keywordName, match, reg))
      {
         if (match[1] == "R") setParameter(keywordName, m_relaysList, command, match[2], parameters);
         else if (match[1] == "C") setParameter(keywordName, m_countersList, command, command->getBody(), parameters);
         else throw shared::exception::CException("Invalid or Read-Only Keyword:" + keywordName);

         // No command should be received for DI and Analog Values
      }*/

      return parameters;
   }

   CWESEquipment::~CWESEquipment()
   {}
}// namespace equipments