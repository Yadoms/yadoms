#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

namespace equipments
{
	const std::map<ContractAvailable, std::vector<std::string>> CTIC::m_counterNames = 
	{ {Base, {"BaseCounter"}},
	  {HpHc, {"LowCostCounter" , "NormalCostCounter"}},
	  {Ejp,  {"EJPPeakPeriod" , "EJPNormalPeriod"}},
	  {Tempo,  {"TempoBlueDaysLowCostPeriod" , "TempoBlueDaysNormalCostPeriod", "TempoRedDaysLowCostPeriod", "TempoRedDaysNormalCostPeriod", "TempoWhiteDaysLowCostPeriod", "TempoWhiteDaysNormalCostPeriod"}},
	};

	const Poco::Int32 CTIC::TICCountersNb = 10;

   CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName,
              const std::string& counterId,
              const ContractAvailable contract) :
      m_deviceName(deviceName),
      m_deviceType("TIC"),
      m_contractName(contract),
      m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
      m_TimePeriod(boost::make_shared<specificHistorizers::CPeriod>("RunningPeriod")),
      m_tomorrowColor(boost::make_shared<specificHistorizers::CColor>("Tomorrow")),
      m_teleInfoStatus(boost::make_shared<specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")),
      m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("DeviceStatus"))
   {
	   shared::CDataContainer details;
	   m_keywords.clear();
	   m_keywords.push_back(m_deviceStatus);

	   // Counters création
	   uint32_t index = 0;
	   for (auto& name : m_counterNames.find(m_contractName)->second) {
		   m_Counter[index] = boost::make_shared<yApi::historization::CEnergy>(name);
		   m_keywords.push_back(m_Counter[index]);
		   ++index;
	   }

	   // Specific déclarations
	   switch (m_contractName)
	   {
	   case Tempo:
		   m_keywords.push_back(m_tomorrowColor);
	   case Base:
	   case HpHc:
	   case Ejp:
	   case HC_Et_WE:
	   case HC_SEM_WE_MERCR:
		   m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);
		   m_keywords.push_back(m_apparentPower);
		   m_keywords.push_back(m_TimePeriod);
		   break;
	   case Producteur:
		   break;
	   case BT4SUP36:
		   break;
	   case BJEJP:
		   break;
	   case NotAvailable:
		   YADOMS_LOG(information) << "This equipment (" << m_deviceName << ") is not available. This keyword is desactivated.";
		   m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kDesactivated);
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
	   api->declareDevice(m_deviceName, "TeleInfo", model, m_keywords, details);
   }

   CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName) :
              m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("DeviceStatus"))
   {
      m_deviceStatus->set(specificHistorizers::EWESdeviceStatus::kTimeOut);
      api->historizeData(deviceName, m_deviceStatus);
   }

   void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                 specificHistorizers::EWESdeviceStatus newState,
                                 const ContractAvailable contractName,
                                 const std::string& counterId,
                                 const int timePeriod,
                                 const unsigned int apparentPower,
	                             const std::vector<Poco::Int64>& counters,
                                 const int newColor)
   {
      setDeviceState(newState);

      // In case of contract change -> create new keywords
      if (m_contractName != contractName){
         m_contractName = contractName;
         //initializeTIC(api, counterId); // TODO : To be changed !
      }

	  auto size = m_counterNames.find(m_contractName)->second.size();
	  for (auto index = 0; index < size; index++) {
		  m_Counter[index]->set(counters[index]);
	  }

      switch (contractName)
      {
      case Base:
      case HpHc:
      case Ejp:
         setPeriodTime(timePeriod);
         break;
      case Tempo:
         setPeriodTime(timePeriod);

         if (m_tomorrowColor->get() != newColor){
            switch (newColor){
            case specificHistorizers::EColor::kNOTDEFINEDValue:
				m_tomorrowColor->set(specificHistorizers::EColor::kNOTDEFINED);
               break;
            case specificHistorizers::EColor::kBLUEValue:
				m_tomorrowColor->set(specificHistorizers::EColor::kBLUE);
               break;
            case specificHistorizers::EColor::kWHITEValue:
				m_tomorrowColor->set(specificHistorizers::EColor::kWHITE);
               break;
            case specificHistorizers::EColor::kREDValue:
				m_tomorrowColor->set(specificHistorizers::EColor::kRED);
               break;
            default:
               break;
            }
         }
         break;
      case BT4SUP36:
         break;
      case BJEJP:
         break;
      case NotAvailable:
         YADOMS_LOG(trace) << "This equipment (" << m_deviceName << ") is desactivated.";
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

   std::string CTIC::name(){
      return m_deviceName;
   }

   void CTIC::setDeviceState(boost::shared_ptr<yApi::IYPluginApi> api,
                              specificHistorizers::EWESdeviceStatus newState)
   {
      if (m_deviceStatus->get() != newState){
         m_deviceStatus->set(newState);
         api->historizeData(m_deviceName, m_deviceStatus);
         YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
      }
   }

   void CTIC::setDeviceState(specificHistorizers::EWESdeviceStatus newState){
      if (m_deviceStatus->get() != newState){
         m_deviceStatus->set(newState);
         YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
      }
   }

   CTIC::~CTIC()
   {}

   void CTIC::setPeriodTime(const int period){
      switch (period){
      case 0:
         if (m_contractName == Base)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kAllHours);
         else if (m_contractName == HpHc || m_contractName == Ejp)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostHours); // TODO : To be validated, but somethig wrong here for HC here
         else if (m_contractName == Tempo)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostBlueDays);
         else if (m_contractName == BT4SUP36)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kWinterNormalCost);
         else if (m_contractName == BJEJP)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kPeakCostHours);
		 else if (m_contractName == HC_Et_WE)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostHours);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostWeek);
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
		 else if (m_contractName == HC_Et_WE)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostHours);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostWeek);
         else
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
         break;
      case 2:
         if (m_contractName == Tempo)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostWhiteDays);
         else if (m_contractName == BT4SUP36 || m_contractName == BJEJP)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kSummerNormalCost);
		 else if (m_contractName == HC_Et_WE)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kWEHours);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostWeekEnd);
         else
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
         break;
      case 3:
         if (m_contractName == Tempo)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostWhiteDays);
         else if (m_contractName == BT4SUP36 || m_contractName == BJEJP)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kSummerLowCost);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostWeekEnd);
         else
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
         break;
      case 4:
         if (m_contractName == Tempo)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostRedDays);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kLowCostDayOfWeek);
         else
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNotDefined);
         break;
      case 5:
         if (m_contractName == Tempo)
            m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostRedDays);
		 else if (m_contractName == HC_SEM_WE_MERCR)
			 m_TimePeriod->set(specificHistorizers::EWESTeleInfoPeriod::kNormalCostDayOfWeek);
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
}// namespace equipments