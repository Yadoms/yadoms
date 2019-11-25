#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

#include "specificHistorizers/Periods/Base.h"
#include "specificHistorizers/Periods/BASEA5.h"
#include "specificHistorizers/Periods/BT4SUP36.h"
#include "specificHistorizers/Periods/BT5SUP36.h"
#include "specificHistorizers/Periods/Ejp.h"
#include "specificHistorizers/Periods/HCWE.h"
#include "specificHistorizers/Periods/HCWeekDayOfWeek.h"
#include "specificHistorizers/Periods/HpHc.h"
#include "specificHistorizers/Periods/HTA5.h"
#include "specificHistorizers/Periods/Tempo.h"
#include "specificHistorizers/Periods/TJEJP.h"

namespace equipments
{
	const std::map<ContractAvailable, std::vector<std::string>> CTIC::m_counterNames = 
	{ {Base, {"Counter"}},
	  {HpHc, {"LowCostPeriod" , "NormalCostPeriod"}},
	  {Ejp,  {"EJPPeakPeriod" , "EJPNormalPeriod"}},
	  {Tempo,  {"TempoBlueDaysLowCostPeriod" , "TempoBlueDaysNormalCostPeriod", "TempoRedDaysLowCostPeriod", "TempoRedDaysNormalCostPeriod", "TempoWhiteDaysLowCostPeriod", "TempoWhiteDaysNormalCostPeriod"}},
	  {Producteur, {"ProductionPeriod"}},
	  {HC_Et_WE, {"LowCostPeriod", "NormalCostPeriod", "LowCostWeekPeriod"}},
	  {HC_SEM_WE_MERCR, {"LowCostWeekPeriod", "NormalCostWeekPeriod", "LowCostWeekEndPeriod", "NormalCostWeekEndPeriod", "LowCostDayOfWeekPeriod", "NormalCostDayOfWeekPeriod"}},
	  {TJEJP, {"MobilePeakPeriod", "WinterPeriod", "SummerNormalPeriod", "SummerLowCostPeriod"}},
	  {BT4SUP36, {"WinterNormalPeriod", "WinterLowCostPeriod", "SummerNormalPeriod", "SummerLowCostPeriod"}},
      {BT5SUP36, {"WinterNormalPeriod", "WinterLowCostPeriod", "SummerNormalPeriod", "SummerLowCostPeriod", "PeakCostPeriod"}},
      {BASEA5, {"WinterNormalPeriod", "WinterLowCostPeriod", "SummerNormalPeriod", "SummerLowCostPeriod"}},
      {HTA5, {"WinterNormalPeriod", "WinterLowCostPeriod", "SummerNormalPeriod", "SummerLowCostPeriod", "PeakCostPeriod"}},
	};

	const Poco::Int32 CTIC::TICCountersNb = 9;

   CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& deviceName,
              const std::string& counterId,
              const ContractAvailable contract) :
      m_deviceName(deviceName),
      m_deviceType("TIC"),
      m_contractName(contract),
      m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
      m_tomorrowColor(boost::make_shared<specificHistorizers::CColor>("Tomorrow")),
      m_teleInfoStatus(boost::make_shared<specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")),
      m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("DeviceStatus"))
   {
	   shared::CDataContainer details;
	   m_keywords.clear();
	   m_keywords.push_back(m_deviceStatus);
	   m_keywords.push_back(m_apparentPower);
	   m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kOk);

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
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodTempo>("RunningPeriod");
		   break;
	   case Base:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodBase>("RunningPeriod");
		   break;
	   case HpHc:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodHpHc>("RunningPeriod");
		   break;
	   case Ejp:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodEjp>("RunningPeriod");
		   break;
	   case Producteur:
		   break;
	   case HC_Et_WE:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodHCWE>("RunningPeriod");
		   break;
	   case HC_SEM_WE_MERCR:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodHCWeekDayofweek>("RunningPeriod");
		   break;
	   case TJEJP:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodTJEJP>("RunningPeriod");
		   break;
	   case BT4SUP36:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodBT4SUP36>("RunningPeriod");
		   break;
	   case BT5SUP36:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodBT5SUP36>("RunningPeriod");
		   break;
	   case BASEA5:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodBaseA5>("RunningPeriod");
		   break;
	   case HTA5:
		   m_runningPeriod = boost::make_shared<specificHistorizers::CPeriodHTA5>("RunningPeriod");
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

	   m_keywords.push_back(m_runningPeriod);
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

	  setPeriodTime(contractName, timePeriod);

      switch (contractName)
      {
	  case Tempo:
		  if (m_tomorrowColor->get() != newColor) {
			  switch (newColor) {
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

   void CTIC::setPeriodTime(const ContractAvailable contractName, const int period){
	   switch (contractName)
	   {
	   case Tempo:
		   m_keywords.push_back(m_tomorrowColor);
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodTempo>(m_runningPeriod)->set((specificHistorizers::EWESPeriodTempo)period);
		   break;
	   case Base:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodBase>(m_runningPeriod)->set((specificHistorizers::EWESPeriodBase)period);
		   break;
	   case HpHc:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodHpHc>(m_runningPeriod)->set((specificHistorizers::EWESPeriodHpHc)period);
		   break;
	   case Ejp:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodEjp>(m_runningPeriod)->set((specificHistorizers::EWESPeriodEjp)period);
		   break;
	   case Producteur:
		   break;
	   case HC_Et_WE:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodHCWE>(m_runningPeriod)->set((specificHistorizers::EWESPeriodHCWE)period);
		   break;
	   case HC_SEM_WE_MERCR:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodHCWeekDayofweek>(m_runningPeriod)->set((specificHistorizers::EWESPeriodHCWeekDayofweek)period);
		   break;
	   case TJEJP:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodTJEJP>(m_runningPeriod)->set((specificHistorizers::EWESPeriodTJEJP)period);
		   break;
	   case BT4SUP36:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodBT4SUP36>(m_runningPeriod)->set((specificHistorizers::EWESPeriodBT4SUP36)period);
		   break;
	   case BT5SUP36:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodBT5SUP36>(m_runningPeriod)->set((specificHistorizers::EWESPeriodBT5SUP36)period);
		   break;
	   case BASEA5:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodBaseA5>(m_runningPeriod)->set((specificHistorizers::EWESPeriodBASEA5)period);
		   break;
	   case HTA5:
		   boost::dynamic_pointer_cast<specificHistorizers::CPeriodHTA5>(m_runningPeriod)->set((specificHistorizers::EWESPeriodHTA5)period);
		   break;
	   case NotAvailable:
		   m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kDesactivated);
		   break;
	   default:
		   YADOMS_LOG(error) << "This contract is unknown";
		   m_teleInfoStatus->set(specificHistorizers::EWESTeleInfoStatus::kDesactivated);
		   break;
	   }
   }
}// namespace equipments