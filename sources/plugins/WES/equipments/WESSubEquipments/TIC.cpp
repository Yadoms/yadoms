#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& deviceName,
                 const std::string& contract) :
         m_deviceName(deviceName),
         m_deviceType("TIC"),
         m_contractName(contract),
         m_baseCounter(boost::make_shared<yApi::historization::CEnergy>("BaseCounter")),
         m_lowCostCounter(boost::make_shared<yApi::historization::CEnergy>("LowCostCounter")),
         m_normalCostCounter(boost::make_shared<yApi::historization::CEnergy>("NormalCostCounter")),
         m_EJPPeakPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPPeakPeriod")),
         m_EJPNormalPeriod(boost::make_shared<yApi::historization::CEnergy>("EJPNormalPeriod")),
         m_tempoBlueDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysLowCostPeriod")),
         m_tempoBlueDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoBlueDaysNormalCostPeriod")),
         m_tempoRedDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysLowCostPeriod")),
         m_tempoRedDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoRedDaysNormalCostPeriod")),
         m_tempoWhiteDaysLowCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysLowCostPeriod")),
         m_tempoWhiteDaysNormalCostPeriod(boost::make_shared<yApi::historization::CEnergy>("TempoWhiteDaysNormalCostPeriod")),
         m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
         m_TimePeriod(boost::make_shared<specificHistorizers::CPeriod>("RunningPeriod")),
         m_teleInfoStatus(boost::make_shared<specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")),
         m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("DeviceStatus"))
      {
         // TODO : Add need to declare
         initializeTIC(api);
      }

      void CTIC::initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         shared::CDataContainer details;
         m_keywords.clear();

         m_keywords.push_back(m_deviceStatus);

         if (m_contractName.compare("Pas Dispo")!=0)
         {
            m_teleInfoStatus->set(specificHistorizers::EStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);

            if (m_contractName.compare("Base")==0)
            {
               m_keywords.push_back(m_baseCounter);
            }
            else if (m_contractName.compare("HP/HC")==0)
            {
               m_keywords.push_back(m_lowCostCounter);
               m_keywords.push_back(m_normalCostCounter);
            }
            else if (m_contractName.compare("EJP")==0)
            {
               m_keywords.push_back(m_EJPPeakPeriod);
               m_keywords.push_back(m_EJPNormalPeriod);
            }
            else if (m_contractName.compare("BBR")==0)
            {
               m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
               m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
               m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
               m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
               m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
               m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
            }
            else
            {
               YADOMS_LOG(error) << "This contract is unknown";
            }
         }
         else
         {
            m_teleInfoStatus->set(specificHistorizers::EStatus::kDesactivated);
         }

         m_keywords.push_back(m_teleInfoStatus);

         std::string model = "TIC";
         details.set("type", m_deviceType);

         //Déclaration of all IOs
         api->declareDevice(m_deviceName, "TeleInfo",model, m_keywords, details);
      }

      void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                  specificHistorizers::EdeviceStatus newState,
                                  const std::string& contractName,
                                  const std::string& timePeriod,
                                  const unsigned int apparentPower,
                                  const Poco::Int64& counter1,
                                  const Poco::Int64& counter2,
                                  const Poco::Int64& counter3,
                                  const Poco::Int64& counter4,
                                  const Poco::Int64& counter5,
                                  const Poco::Int64& counter6)
      {
         m_deviceStatus->set(newState);

         // In case of contract change -> create new keywords
         if (m_contractName.compare(contractName) != 0)
         {
            m_contractName = contractName;
            initializeTIC(api);
         }

         if (m_contractName.compare("Base") == 0)
         {
            m_baseCounter->set(counter1);
         }
         else if (m_contractName.compare("HP/HC") == 0)
         {
            m_lowCostCounter->set(counter1);
            m_normalCostCounter->set(counter2);
         }
         else if (m_contractName.compare("EJP") == 0)
         {
            m_EJPPeakPeriod->set(counter1);
            m_EJPNormalPeriod->set(counter2);
         }
         else if (m_contractName.compare("BBR") == 0)
         {
            m_tempoBlueDaysLowCostPeriod->set(counter1);
            m_tempoBlueDaysNormalCostPeriod->set(counter2);
            m_tempoRedDaysLowCostPeriod->set(counter3);
            m_tempoRedDaysNormalCostPeriod->set(counter4);
            m_tempoWhiteDaysLowCostPeriod->set(counter5);
            m_tempoWhiteDaysNormalCostPeriod->set(counter6);
         }
         else
         {
            YADOMS_LOG(error) << "This contract is unknown";
         }

         YADOMS_LOG(information) << "Time period :" << timePeriod;

         // Time Period
         // TODO : Ne doit pas être appliquer si rien d'actif
         if (timePeriod.compare("Toutes Heures") == 0)
         {
            m_TimePeriod->set(specificHistorizers::EPeriod::kAllHours);
         }
         else
         {
            YADOMS_LOG(error) << "This time period is unknown";
         }

         m_apparentPower->set(apparentPower);

         api->historizeData(m_deviceName, m_keywords);
      }

      void CTIC::remove(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         // remove the device from the database
         api->removeDevice(m_deviceName);
         YADOMS_LOG(information) << "remove " << m_deviceName;
      }

      std::string CTIC::name()
      {
         return m_deviceName;
      }

      void CTIC::setDeviceState(boost::shared_ptr<yApi::IYPluginApi> api,
                                specificHistorizers::EdeviceStatus newState)
      {
         setDeviceState(newState);
         api->historizeData(m_deviceName, m_deviceStatus); // TODO : historize only if the status change
      }

      void CTIC::setDeviceState(specificHistorizers::EdeviceStatus newState)
      {
         if (m_deviceStatus->get() != newState)
         {
            m_deviceStatus->set(newState);
            YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
         }
      }

      CTIC::~CTIC()
      {}
   }
}// namespace equipments::subdevices