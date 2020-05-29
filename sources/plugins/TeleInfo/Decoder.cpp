#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>

const std::string CDecoder::m_tag_ADCO = "ADCO"; //meter id
const std::string CDecoder::m_tag_OPTARIF = "OPTARIF";//pricing option
const std::string CDecoder::m_tag_BASE = "BASE";//total power usage normal tariff in base option
const std::string CDecoder::m_tag_HCHC = "HCHC";// total power usage low tariff in HC option
const std::string CDecoder::m_tag_HCHP = "HCHP";// total power usage normal tariff in HC option
const std::string CDecoder::m_tag_EJPHPM = "EJPHPM";// total power usage normal tariff in PM option
const std::string CDecoder::m_tag_EJPHN = "EJPHN";// total power usage low tariff in HN option
const std::string CDecoder::m_tag_BBRHCJB = "BBRHCJB";// total power usage low tariff in HC option tempo blue
const std::string CDecoder::m_tag_BBRHPJB = "BBRHPJB";// total power usage normal tariff in HC option tempo blue
const std::string CDecoder::m_tag_BBRHCJW = "BBRHCJW";// total power usage low tariff in HC option tempo white
const std::string CDecoder::m_tag_BBRHPJW = "BBRHPJW";// total power usage normal tariff in HC option tempo white
const std::string CDecoder::m_tag_BBRHCJR = "BBRHCJR";// total power usage low tariff in HC option tempo red
const std::string CDecoder::m_tag_BBRHPJR = "BBRHPJR";// total power usage normal tariff in HC option tempo red
const std::string CDecoder::m_tag_PTEC = "PTEC";//current tariff period
const std::string CDecoder::m_tag_IINST = "IINST";//instant current power usage
const std::string CDecoder::m_tag_IINST1 = "IINST1";//instant current power usage phase 1
const std::string CDecoder::m_tag_IINST2 = "IINST2";//instant current power usage phase 2
const std::string CDecoder::m_tag_IINST3 = "IINST3";//instant current power usage phase 3
const std::string CDecoder::m_tag_PAPP = "PAPP";//apparent power
const std::string CDecoder::m_tag_DEMAIN = "DEMAIN"; // Color of the next day
const std::string CDecoder::m_tag_PEJP = "PEJP"; // EJP Warning

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api) :
   m_DeviceDetails(shared::CDataContainer::make()),
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
   m_instantCurrent(boost::make_shared<yApi::historization::CCurrent>("InstantCurrent")),
   m_apparentPower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
   m_TimePeriod(boost::make_shared<CRunningPeriod>(api, "RunningPeriod")),
   m_ForecastPeriod(boost::make_shared<CForecastTomorrow>(api, "ForecastColor")),
   m_warningEJP(boost::make_shared<yApi::historization::CSwitch>("warningEJP")),
   m_api(api),
   m_teleinfoEnableInCounter(false),
   m_deviceCreated(false),
   m_ADCOalreadyReceived(false),
   m_optarif(OP_NOT_DEFINED),
   m_newWarningEJPValue(false),
   m_firstRun(true)
{
   m_instantCurrentPhase[0] = boost::make_shared<yApi::historization::CCurrent>("InstantCurrentPhase1");
   m_instantCurrentPhase[1] = boost::make_shared<yApi::historization::CCurrent>("InstantCurrentPhase2");
   m_instantCurrentPhase[2] = boost::make_shared<yApi::historization::CCurrent>("InstantCurrentPhase3");
}

CDecoder::~CDecoder()
{
}

void CDecoder::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{
   bool isMono = true; // default mono

   m_teleinfoEnableInCounter = (messages->size() == 1 && messages->find(m_tag_ADCO) != messages->end()) ? false : true;

   // By default (for EJP/Tempo), the forecast is not defined, if not present
   // It's not used for Base or HPHC contract
   m_ForecastPeriod->set(teleInfo::specificHistorizers::EColor::kNOTDEFINED);
   m_newWarningEJPValue = false;

   for (const auto message : *messages)
   {
      processMessage(message.first,
                     message.second);
   }

   //tree phases
   if (messages->find(m_tag_IINST1) != messages->end() && 
       messages->find(m_tag_IINST2) != messages->end() &&
       messages->find(m_tag_IINST3) != messages->end()
      )
   {
      isMono = false;
   }

   // compute the PAPP if this one is not present
   if (messages->find(m_tag_PAPP) == messages->end())
   {
      if (isMono)
         m_apparentPower->set(m_instantCurrent->get() * 230);
      else
         m_apparentPower->set((m_instantCurrentPhase[0]->get() +
                               m_instantCurrentPhase[1]->get() +
                               m_instantCurrentPhase[2]->get()
                               ) * 230);
   }

   if (!m_deviceCreated)
      createDeviceAndKeywords(isMono);
   else
      constructKeywordList(m_optarif, isMono);

   m_api->historizeData(m_deviceName, m_keywords);
}

void CDecoder::createDeviceAndKeywords(const bool monoPhase)
{
   YADOMS_LOG(trace) << "Nb keywords : " << "=" << m_keywords.size() ;

   if (monoPhase)
   {
      m_keywords.push_back(m_instantCurrent);
   }
   else
   {
      for (unsigned char counter = 0; counter < 3; ++counter)
         m_keywords.push_back(m_instantCurrentPhase[counter]);
   }

   if (!isERDFCounterDesactivated() && m_deviceName!="")
   {
      m_api->declareDevice(m_deviceName, "teleInfoUSB", 
                           "TeleInfoUSB : Id = " + m_deviceName,
                           m_keywords,
                           m_DeviceDetails);

      // for compatibility with old plugin revision, we create separately the keyword if not existing
      // If the device already exist, we have to create keywords manually
      // This is only for EJP contracts
      if (!m_api->keywordExists(m_deviceName, m_warningEJP) && m_optarif == OP_EJP)
         m_api->declareKeyword(m_deviceName, m_warningEJP);

      m_deviceCreated = true;
      m_firstRun = false;
   }

   if (m_deviceName == "")
      YADOMS_LOG(debug) << "Device is null during registration";
}

void CDecoder::constructKeywordList(const EContracts contract, const bool monoPhase)
{
   m_keywords.clear();

   switch (contract)
   {
   case OP_BASE:
   {
      m_keywords.push_back(m_baseCounter);
      m_keywords.push_back(m_apparentPower);
      if (m_TimePeriod->isChanged()) m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case OP_CREUSE:
   {
      
      m_keywords.push_back(m_lowCostCounter);
      m_keywords.push_back(m_normalCostCounter);
      m_keywords.push_back(m_apparentPower);
      if (m_TimePeriod->isChanged()) m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case OP_EJP:
   {
      m_keywords.push_back(m_EJPPeakPeriod);
      m_keywords.push_back(m_EJPNormalPeriod);
      m_keywords.push_back(m_apparentPower);
      if (m_TimePeriod->isChanged()) m_keywords.push_back(m_TimePeriod->GetHistorizable());
      if (m_newWarningEJPValue != m_warningEJP->get() || m_firstRun)
      {
         m_warningEJP->set(m_newWarningEJPValue);
         m_keywords.push_back(m_warningEJP);
      }
      break;
   }
   case OP_TEMPO:
   {
      m_keywords.push_back(m_apparentPower);
      m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
      m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
      m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
      if (m_TimePeriod->isChanged()) m_keywords.push_back(m_TimePeriod->GetHistorizable());
      if (m_ForecastPeriod->isChanged()) m_keywords.push_back(m_ForecastPeriod->GetHistorizable());
      break;
   }
   default:
      //Erreur normalement
      break;
   }

   if (monoPhase)
   {
      m_keywords.push_back(m_instantCurrent);
   }
   else
   {
      for (unsigned char counter = 0; counter < 3; ++counter)
         m_keywords.push_back(m_instantCurrentPhase[counter]);
   }
}

void CDecoder::declareKeywordList(const std::string& tariff)
{
   m_keywords.clear();

   switch (tariff[1])
   {
   case 'A':
   {
      m_optarif = OP_BASE;

      m_keywords.push_back(m_baseCounter);
      m_keywords.push_back(m_apparentPower);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case 'C':
   {
      m_optarif = OP_CREUSE;

      m_keywords.push_back(m_lowCostCounter);
      m_keywords.push_back(m_normalCostCounter);
      m_keywords.push_back(m_apparentPower);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case 'J':
   {
      m_optarif = OP_EJP;

      m_keywords.push_back(m_EJPPeakPeriod);
      m_keywords.push_back(m_EJPNormalPeriod);
      m_keywords.push_back(m_apparentPower);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      m_keywords.push_back(m_ForecastPeriod->GetHistorizable());
      break;
   }
   case 'B':
   {
      m_optarif = OP_TEMPO;

      m_keywords.push_back(m_apparentPower);
      m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
      m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
      m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      m_keywords.push_back(m_ForecastPeriod->GetHistorizable());
      break;
   }
   default:
      //Erreur normalement
      break;
   }
}

bool CDecoder::isERDFCounterDesactivated() const
{
   return !m_teleinfoEnableInCounter;
}

void CDecoder::processMessage(const std::string& key,
                              const std::string& value)
{
	try
	{
		if (key == m_tag_ADCO)
		{
			YADOMS_LOG(trace) << "ADCO" << "=" << value ;

			if (!m_ADCOalreadyReceived)
			{
            YADOMS_LOG(debug) << "m_deviceName = " << value;
				m_deviceName = value;
				m_ADCOalreadyReceived = true;
			}
		}
		else if (key == m_tag_OPTARIF)
		{
			YADOMS_LOG(trace) << "OPTARIF" << "=" << value ;
			if (m_keywords.empty())
				declareKeywordList(value);
		}
		else if (key == m_tag_BASE)
		{
			YADOMS_LOG(trace) << "BASE" << "=" << value ;
			m_baseCounter->set(std::stoll(value));
		}
		else if (key == m_tag_HCHC)
		{
			YADOMS_LOG(trace) << "HCHC" << "=" << value ;
			m_lowCostCounter->set(std::stoll(value));
		}
		else if (key == m_tag_HCHP)
		{
			YADOMS_LOG(trace) << "HCHP" << "=" << value ;
			m_normalCostCounter->set(std::stoll(value));
		}
		else if (key == m_tag_EJPHPM)
		{
			YADOMS_LOG(trace) << "EJPHPM" << "=" << value ;
			m_EJPPeakPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_EJPHN)
		{
			YADOMS_LOG(trace) << "EJPHN" << "=" << value ;
			m_EJPNormalPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHCJB)
		{
			YADOMS_LOG(trace) << "BBRHCJB" << "=" << value ;
			m_tempoBlueDaysLowCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHPJB)
		{
			YADOMS_LOG(trace) << "BBRHPJB" << "=" << value ;
			m_tempoBlueDaysNormalCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHCJW)
		{
			YADOMS_LOG(trace) << "BBRHCJW" << "=" << value ;
			m_tempoWhiteDaysLowCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHPJW)
		{
			YADOMS_LOG(trace) << "BBRHPJW" << "=" << value ;
			m_tempoWhiteDaysNormalCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHCJR)
		{
			YADOMS_LOG(trace) << "BBRHCJR" << "=" << value ;
			m_tempoRedDaysLowCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_BBRHPJR)
		{
			YADOMS_LOG(trace) << "BBRHPJR" << "=" << value ;
			m_tempoRedDaysNormalCostPeriod->set(std::stoll(value));
		}
		else if (key == m_tag_PTEC)
		{
			YADOMS_LOG(trace) << "PTEC" << "=" << value ;
			m_TimePeriod->set(value);
		}
		else if (key == m_tag_IINST)
		{
			YADOMS_LOG(trace) << "IINST" << "=" << value ;
			m_instantCurrent->set(std::stod(value));
		}
      else if (key == m_tag_IINST1)
      {
         YADOMS_LOG(trace) << "IINST1" << "=" << value;
         m_instantCurrentPhase[0]->set(std::stod(value));
      }
      else if (key == m_tag_IINST2)
      {
         YADOMS_LOG(trace) << "IINST2" << "=" << value;
         m_instantCurrentPhase[1]->set(std::stod(value));
      }
      else if (key == m_tag_IINST3)
      {
         YADOMS_LOG(trace) << "IINST3" << "=" << value;
         m_instantCurrentPhase[2]->set(std::stod(value));
      }
		else if (key == m_tag_PAPP)
		{
			YADOMS_LOG(trace) << "PAPP" << "=" << value ;
			m_apparentPower->set(std::stol(value));
		}
		else if (key == m_tag_DEMAIN)
		{
			YADOMS_LOG(trace) << "DEMAIN" << "=" << value ;
			m_ForecastPeriod->set(value);
		}
      else if (key == m_tag_PEJP)
      {
         YADOMS_LOG(information) << "PEJP" << "=" << value;
         m_newWarningEJPValue = true;
      }
		else
		{
			YADOMS_LOG(warning) << "label " << key << " not processed" ;
		}
	}
	catch (std::exception& e )
	{
		YADOMS_LOG(error) << "Exception received !" << e.what() ;
	}
}