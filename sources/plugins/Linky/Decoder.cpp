#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include <boost/algorithm/string.hpp>

const std::string CDecoder::m_tag_ADSC = "ADSC";  //meter id
const std::string CDecoder::m_tag_VTIC = "VTIC";  //Linky revision
const std::string CDecoder::m_tag_NGTF = "NGTF";  // current tariff period
const std::string CDecoder::m_tag_LTARF = "LTARF"; // 
const std::string CDecoder::m_tag_EASF = "EASF";  // counter ...
const std::string CDecoder::m_tag_EASD = "EASD";// counter

DECLARE_ENUM_IMPLEMENTATION(EContract,
((BASE))
((CREUSE))
((EJP))
((TEMPO))
);

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
   : m_baseCounter(boost::make_shared<yApi::historization::CEnergy>("BaseCounter")),
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
     m_api(api),
     m_isdeveloperMode(false),
     m_linkyEnableInCounter(false),
     m_deviceCreated(false),
     m_optarif(OP_NOT_DEFINED)
{
   m_isdeveloperMode = api->getYadomsInformation()->developperMode();
}

CDecoder::~CDecoder()
{
}

void CDecoder::decodeLinkyMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{
   // To be checked
   //m_linkyEnableInCounter = (messages->size() == 1 && messages->find(m_tag_ADSC) != messages->end()) ? false : true;

   for (const auto message : *messages)
   {
      processMessage(message.first,
                     message.second);
   }

   if (!m_deviceCreated)
      createDeviceAndKeywords();

   m_api->historizeData(m_deviceName, m_keywords);
}

void CDecoder::createDeviceAndKeywords()
{
   if (m_isdeveloperMode) YADOMS_LOG(information) << "Nb keywords : " << "=" << m_keywords.size() ;

   m_api->declareDevice(m_deviceName, "teleInfoUSB", 
                        "TeleInfoUSB : Id = " + m_deviceName,
                        m_keywords,
                        m_DeviceDetails);

   m_deviceCreated = true;
}

void CDecoder::createKeywordList(const std::string& tariff)
{
   switch (tariff[1])
   {
   case 'A':
   {
      m_optarif = OP_BASE;

      m_keywords.clear();
      m_keywords.push_back(m_baseCounter);
      m_keywords.push_back(m_instantCurrent);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case 'C':
   {
      m_optarif = OP_CREUSE;

      m_keywords.clear();
      m_keywords.push_back(m_lowCostCounter);
      m_keywords.push_back(m_normalCostCounter);
      m_keywords.push_back(m_instantCurrent);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case 'J':
   {
      m_optarif = OP_EJP;

      m_keywords.clear();
      m_keywords.push_back(m_EJPPeakPeriod);
      m_keywords.push_back(m_EJPNormalPeriod);
      m_keywords.push_back(m_instantCurrent);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   case 'B':
   {
      m_optarif = OP_TEMPO;

      m_keywords.clear();
      m_keywords.push_back(m_tempoBlueDaysLowCostPeriod);
      m_keywords.push_back(m_tempoBlueDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoRedDaysLowCostPeriod);
      m_keywords.push_back(m_tempoRedDaysNormalCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysLowCostPeriod);
      m_keywords.push_back(m_tempoWhiteDaysNormalCostPeriod);
      m_keywords.push_back(m_instantCurrent);
      m_keywords.push_back(m_TimePeriod->GetHistorizable());
      break;
   }
   default:
      //Erreur normalement
      break;
   }
}

void CDecoder::processMessage(const std::string& key,
                                  const std::string& value)
{
	try
	{
		if (key == m_tag_ADSC)
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << "ADSC" << "=" << value ;

			static bool ADSCalreadyReceived = false;

			if (!ADSCalreadyReceived)
			{
				m_deviceName = value;
				ADSCalreadyReceived = true;
			}
		}
		else if (key == m_tag_VTIC)
		{
         m_revision = value;
			if (m_isdeveloperMode) YADOMS_LOG(information) << "VTIC" << "=" << value ;
			if (m_keywords.empty())
				createKeywordList(value);
		}
		else if (key == m_tag_NGTF)
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << "NGTF" << "=<" << value << ">";
         std::string s_period(value);
         boost::trim_right(s_period);
         boost::trim_left(s_period);
         teleInfo::specificHistorizers::EPeriod period(s_period); //TODO : Changer le nom teleInfo en linky
         m_TimePeriod->set(period);
		}
		else if (key == m_tag_LTARF)
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << "LTARF" << "=<" << value << ">";
         std::string s_contract(value);
         boost::trim_right(s_contract);
         boost::trim_left(s_contract);
         EContract contract(s_contract);
		}
		else if (key.find(m_tag_EASF))
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << m_tag_EASF << "=" << value ;
		}
		else if (key.find(m_tag_EASD))
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << m_tag_EASD << "=" << value ;
		}
		else
		{
			YADOMS_LOG(error) << "label " << key << " not processed" ;
		}
	}
	catch (std::exception& e )
	{
		YADOMS_LOG(error) << "Exception received !" << e.what() ;
	}
}