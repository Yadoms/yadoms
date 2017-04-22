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
         m_teleInfoStatus(boost::make_shared<specificHistorizers::CTeleInfoStatus>("Status"))
      {
         // TODO : Add need to declare
         initializeTIC(api);
      }

      void CTIC::initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         shared::CDataContainer details;

         if (m_contractName.compare("Pas Dispo")!=0)
         {
            m_teleInfoStatus->set(specificHistorizers::EStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);

            if (m_contractName.compare("BASE")==0)
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

         std::string model = "WES";

         //Déclaration of all IOs
         api->declareDevice(m_deviceName, "TeleInfo",model, m_keywords, details);
      }

      std::string CTIC::getDeviceName() const
      {
         return m_deviceName;
      }

      void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const long long& counter1,
                                  const long long& counter2,
                                  const long long& counter3,
                                  const long long& counter4,
                                  const long long& counter5,
                                  const long long& counter6)
      {
         //TODO : If deviceName or contractName are different then create a new device
         //initializeTIC(api);

         if (m_contractName.compare("BASE") == 0)
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

         api->historizeData(m_deviceName, m_keywords);
      }

      CTIC::~CTIC()
      {}
   }
}// namespace equipments::subdevices