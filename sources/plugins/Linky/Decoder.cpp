#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>
#include "specificHistorizer/Color.h"

const std::string CDecoder::m_tag_ADSC = "ADSC";    // meter id
const std::string CDecoder::m_tag_VTIC = "VTIC";    // Linky revision
const std::string CDecoder::m_tag_NGTF = "NGTF";    // current tariff period
const std::string CDecoder::m_tag_LTARF = "LTARF";  // 
const std::string CDecoder::m_tag_EASF = "EASF";    // counter ...
const std::string CDecoder::m_tag_EASD = "EASD";    // counter
const std::string CDecoder::m_tag_STGE = "STGE";    // Status 32 bits word
const std::string CDecoder::m_tag_EAIT = "EAIT";    // counter active energy injected to the network
const std::string CDecoder::m_tag_SINST1 = "SINST1";// apparent power for phase 1
const std::string CDecoder::m_tag_SINST2 = "SINST2";// apparent power for phase 2
const std::string CDecoder::m_tag_SINST3 = "SINST3";// apparent power for phase 3
const std::string CDecoder::m_tag_SINSTS = "SINSTS";// global apparent power
const std::string CDecoder::m_tag_UMOY1  = "UMOY1"; // mean voltage for phase 1
const std::string CDecoder::m_tag_UMOY2  = "UMOY2"; // mean voltage for phase 2
const std::string CDecoder::m_tag_UMOY3  = "UMOY3"; // mean voltage for phase 3

DECLARE_ENUM_IMPLEMENTATION(EContract,
((BASE))
((CREUSE))
((EJP))
((TEMPO))
);

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
   :
   m_activeEnergyInjected(boost::make_shared<yApi::historization::CEnergy>("ActiveEnergyInjected")),
   m_TimePeriod(boost::make_shared<linky::specificHistorizers::CPeriod>("runningPeriod")),
   m_forecastTomorrow(boost::make_shared<linky::specificHistorizers::CColor>("TomorrowColor")),
   m_forecastToday(boost::make_shared<linky::specificHistorizers::CColor>("TodayColor")),
     m_api(api),
     m_isdeveloperMode(false),
     m_linkyEnableInCounter(false),
     m_deviceCreated(false),
     m_optarif(OP_NOT_DEFINED),
   m_indexFournisseurActif(0),
   m_producteur(false)
{
   m_isdeveloperMode = api->getYadomsInformation()->developperMode();

   for (int counter = 0; counter < 3; ++counter)
      m_apparentPower[counter] = boost::make_shared<yApi::historization::CApparentPower>("ApparentPower" + boost::lexical_cast<std::string>(counter + 1));

   for (int counter = 0; counter < 10; ++counter)
      m_counter[counter] = boost::make_shared<yApi::historization::CEnergy>("Counter" + boost::lexical_cast<std::string>(counter+1));
}

CDecoder::~CDecoder()
{
}

void CDecoder::decodeLinkyMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{
   // TODO : To be checked
   //m_linkyEnableInCounter = (messages->size() == 1 && messages->find(m_tag_ADSC) != messages->end()) ? false : true;
   bool triphases = (messages->find(m_tag_SINST2) != messages->end()) ? false : true;

   for (const auto message : *messages)
   {
      processMessage(message.first,
                     message.second);
   }

   if (!m_deviceCreated)
      createDeviceAndKeywords(triphases);

   m_api->historizeData(m_deviceName, m_keywords);
}

void CDecoder::createDeviceAndKeywords(bool isTriphases)
{
   if (m_isdeveloperMode) YADOMS_LOG(information) << "Nb keywords : " << "=" << m_keywords.size() ;

   if (isTriphases) // We add missing phases
   {
      m_keywords.push_back(m_apparentPower[1]);
      m_keywords.push_back(m_apparentPower[2]);
      m_keywords.push_back(m_meanVoltage[1]);
      m_keywords.push_back(m_meanVoltage[2]);
   }

   m_api->declareDevice(m_deviceName, "linkyUSB", 
                        "linkyUSB : Id = " + m_deviceName,
                        m_keywords,
                        m_DeviceDetails);

   m_deviceCreated = true;
}

void CDecoder::createKeywordList(const std::string& tariff)
{
   m_keywords.clear();

   switch (tariff[1])
   {
   case 'A':
   {
      m_optarif = OP_BASE;
      break;
   }
   case 'C':
   {
      m_optarif = OP_CREUSE;
      break;
   }
   case 'J':
   {
      m_optarif = OP_EJP;
      break;
   }
   case 'B':
   {
      m_optarif = OP_TEMPO;
      m_keywords.push_back(m_forecastTomorrow);
      m_keywords.push_back(m_forecastToday);
      break;
   }
   default:
      //Erreur normalement
      break;
   }

   if (tariff[1] == 'A' || tariff[1] == 'C' || tariff[1] == 'J' || tariff[1] == 'B')
   {
      // common for all contracts
      m_keywords.push_back(m_apparentPower[0]);
      m_keywords.push_back(m_meanVoltage[0]);
      m_keywords.push_back(m_TimePeriod);

      for (unsigned char counter = 0; counter < 10; ++counter)
      {
         if (m_counter[counter]->get() != 0) // We register only counter != 0
            m_keywords.push_back(m_counter[counter]);
      }
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
         m_revision = boost::lexical_cast<unsigned char>(value);
			if (m_isdeveloperMode) YADOMS_LOG(information) << "VTIC" << "=" << value ;
		}
		else if (key == m_tag_NGTF)
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << "NGTF" << "=<" << value << ">";
         std::string s_period(value);
         boost::trim_right(s_period);
         boost::trim_left(s_period);
         linky::specificHistorizers::EPeriod period(s_period);
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
         
         //separate the counter number from the name
         boost::regex reg("([A-Z]{4})\\d{2}");
         boost::smatch match;

         if (boost::regex_search(key, match, reg))
         {
            unsigned long counterIndex = boost::lexical_cast<unsigned long>(match[2]);
            m_counter[counterIndex - 1]->set(boost::lexical_cast<unsigned long>(value));
         }
		}
		else if (key.find(m_tag_EASD)) //TODO : To be deleted if any
		{
			if (m_isdeveloperMode) YADOMS_LOG(information) << m_tag_EASD << "=" << value ;
		}
      else if (key.find(m_tag_STGE))
      {
         uint32_t status = 0;

         // Convert Hexa string to value
         std::stringstream ss;
         ss << std::hex << value;
         ss >> status;

         // TODO : rename french names

         m_indexFournisseurActif = (status & 0x00003C00) >> 10; // bits 10 to 13

         if (status & 0x00000100) // bit 8
            m_producteur = true;
         else
            m_producteur = false;

         m_forecastToday->set(linky::specificHistorizers::EColor((status >> 24) & 0x03)); // bit 24 to 25 color of today
         m_forecastTomorrow->set(linky::specificHistorizers::EColor((status & 0x0C000000) >> 26)); // bits 26 to 27 color of tomorrow

         // Creation of keywords here with the STGE near the end of the frame
         if (m_keywords.empty())
            createKeywordList(value);
      }
      else if (key.find(m_tag_EAIT))
      {
         m_activeEnergyInjected->set(boost::lexical_cast<unsigned long>(value));
      }
      else if (key.find(m_tag_SINST2))
      {
         m_apparentPower[1]->set(boost::lexical_cast<double>(value));
      }
      else if (key.find(m_tag_SINST3))
      {
         m_apparentPower[2]->set(boost::lexical_cast<double>(value));
      }
      else if (m_revision == 1) // specific functions v1
      {
         if (key.find(m_tag_SINST1))
            m_apparentPower[0]->set(boost::lexical_cast<double>(value));
      }
      else if (m_revision == 2) // specific functions v2
      {
         if (key.find(m_tag_SINSTS))
            m_apparentPower[0]->set(boost::lexical_cast<double>(value));
         else
         {
         }
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