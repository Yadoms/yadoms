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
         initializeTIC(api);
      }

      void CTIC::initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
         shared::CDataContainer details;

         if (m_contractName.compare("Pas Dispo")!=0)
         {
            m_teleInfoStatus->set(specificHistorizers::EStatus::kOk);
            keywordsToDeclare.push_back(m_apparentPower);
            keywordsToDeclare.push_back(m_TimePeriod);

            if (m_contractName.compare("BASE")==0)
            {
               keywordsToDeclare.push_back(m_baseCounter);
            }
            else if (m_contractName.compare("HP/HC")==0)
            {
               keywordsToDeclare.push_back(m_lowCostCounter);
               keywordsToDeclare.push_back(m_normalCostCounter);
            }
            else if (m_contractName.compare("EJP")==0)
            {
               keywordsToDeclare.push_back(m_EJPPeakPeriod);
               keywordsToDeclare.push_back(m_EJPNormalPeriod);
            }
            else if (m_contractName.compare("BBR")==0)
            {
               keywordsToDeclare.push_back(m_tempoBlueDaysLowCostPeriod);
               keywordsToDeclare.push_back(m_tempoBlueDaysNormalCostPeriod);
               keywordsToDeclare.push_back(m_tempoRedDaysLowCostPeriod);
               keywordsToDeclare.push_back(m_tempoRedDaysNormalCostPeriod);
               keywordsToDeclare.push_back(m_tempoWhiteDaysLowCostPeriod);
               keywordsToDeclare.push_back(m_tempoWhiteDaysNormalCostPeriod);
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

         keywordsToDeclare.push_back(m_teleInfoStatus);

         //Déclaration of all IOs
         api->declareDevice(m_deviceName, "TeleInfo", keywordsToDeclare, details);
      }

      std::string CTIC::getDeviceName() const
      {
         return m_deviceName;
      }

      void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api)
      {
         //TODO : If deviceName or contractName are different then create a new device
         //initializeTIC(api);
      }

      CTIC::~CTIC()
      {}
   }
}// namespace equipments::subdevices