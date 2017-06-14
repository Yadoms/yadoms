#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

namespace equipments
{
   namespace subdevices
   {
      CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                 const std::string& deviceName,
                 const std::string& counterId,
                 const ContractAvailable contract) :
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
         m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("DeviceStatus")),
         m_Color(boost::make_shared<specificHistorizers::CColor>("Tomorrow"))
      {
         initializeTIC(api, counterId);
      }

      void CTIC::initializeTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& counterId)
      {
         shared::CDataContainer details;
         m_keywords.clear();

         m_keywords.push_back(m_deviceStatus);

         switch (m_contractName)
         {
         case Base:
            m_keywords.push_back(m_baseCounter);
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);
            break;
         case HpHc:
            m_keywords.push_back(m_lowCostCounter);
            m_keywords.push_back(m_normalCostCounter);
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);
            break;
         case Ejp:
            m_keywords.push_back(m_EJPPeakPeriod);
            m_keywords.push_back(m_EJPNormalPeriod);
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);
            break;
         case Tempo:
            m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
            m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
            m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
            m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
            m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
            m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);
            m_keywords.push_back(m_apparentPower);
            m_keywords.push_back(m_TimePeriod);
            m_keywords.push_back(m_Color);
            break;
         case BT4SUP36:
            break;
         case BJEJP:
            break;
         default:
            YADOMS_LOG(error) << "This contract is unknown";
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kDesactivated);
            break;
         }

         m_keywords.push_back(m_teleInfoStatus);

         std::string model = "TIC Id = " + counterId;
         details.set("type", m_deviceType);

         //Déclaration of all IOs
         api->declareDevice(m_deviceName, "TeleInfo",model, m_keywords, details);
      }

      void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                  specificHistorizers::EWESdeviceStatus newState,
                                  const ContractAvailable contractName,
                                  const std::string& counterId,
                                  const int timePeriod,
                                  const unsigned int apparentPower,
                                  const Poco::Int64& counter1,
                                  const Poco::Int64& counter2,
                                  const Poco::Int64& counter3,
                                  const Poco::Int64& counter4,
                                  const Poco::Int64& counter5,
                                  const Poco::Int64& counter6,
                                  const specificHistorizers::EColor newColor)
      {
         m_deviceStatus->set(newState);

         // In case of contract change -> create new keywords
         if (m_contractName != contractName)
         {
            m_contractName = contractName;
            initializeTIC(api, counterId);
         }

         switch (contractName)
         {
         case Base:
            m_baseCounter->set(counter1);
            setPeriodTime(timePeriod);

            break;
         case HpHc:
            m_lowCostCounter->set(counter1);
            m_normalCostCounter->set(counter2);
            setPeriodTime(timePeriod);

            break;
         case Ejp:
            m_EJPPeakPeriod->set(counter1);
            m_EJPNormalPeriod->set(counter2);
            setPeriodTime(timePeriod);

            break;
         case Tempo:
            m_tempoBlueDaysLowCostPeriod->set(counter1);
            m_tempoBlueDaysNormalCostPeriod->set(counter2);
            m_tempoRedDaysLowCostPeriod->set(counter3);
            m_tempoRedDaysNormalCostPeriod->set(counter4);
            m_tempoWhiteDaysLowCostPeriod->set(counter5);
            m_tempoWhiteDaysNormalCostPeriod->set(counter6);
            setPeriodTime(timePeriod);

            if (m_Color->get() != newColor)
               m_Color->set(newColor);

            break;
         case BT4SUP36:
            break;
         case BJEJP:
            break;
         default:
            YADOMS_LOG(error) << "This contract is unknown";
            break;
         }

         YADOMS_LOG(information) << "Time period :" << timePeriod;

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
                                specificHistorizers::EWESdeviceStatus newState)
      {
         if (m_deviceStatus->get() != newState)
         {
            m_deviceStatus->set(newState);
            api->historizeData(m_deviceName, m_deviceStatus);
            YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
         }
      }

      void CTIC::setDeviceState(specificHistorizers::EWESdeviceStatus newState)
      {
         if (m_deviceStatus->get() != newState)
         {
            m_deviceStatus->set(newState);
            YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
         }
      }

      CTIC::~CTIC()
      {}

      void CTIC::setPeriodTime(const int period)
      {
         switch (period)
         {
         case 0:
            if (m_contractName == Base)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kAllHours);
            else if (m_contractName == HpHc || m_contractName == Ejp)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostHours);
            else if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostBlueDays);
            else if (m_contractName == BT4SUP36)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kWinterNormalCost);
            else if (m_contractName == BJEJP)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kPeakCostHours);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 1:
            if (m_contractName == HpHc)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostHours);
            else if (m_contractName == Ejp)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kPeakCostHours);
            else if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostBlueDays);
            else if (m_contractName == BT4SUP36)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kWinterLowCost);
            else if (m_contractName == BJEJP)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kWinterCost);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 2:
            if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostWhiteDays);
            else if (m_contractName == BT4SUP36 || m_contractName == BJEJP)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kSummerNormalCost);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 3:
            if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostWhiteDays);
            else if (m_contractName == BT4SUP36 || m_contractName == BJEJP)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kSummerLowCost);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 4:
            if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostRedDays);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 5:
            if (m_contractName == Tempo)
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostRedDays);
            else
               m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         case 7:
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kError);
            break;
         case 8:
            m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kTeleInfoLost);
            break;
         default:
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
            break;
         }
      }
   }
}// namespace equipments::subdevices