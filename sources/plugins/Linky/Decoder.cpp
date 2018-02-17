#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include <boost/algorithm/string.hpp>
#include "specificHistorizer/Color.h"
#include "LinkyHelpers.hpp"

const std::string CDecoder::m_tag_ADSC = "ADSC";    // meter id
const std::string CDecoder::m_tag_VTIC = "VTIC";    // Linky revision
const std::string CDecoder::m_tag_LTARF = "LTARF";  // Running period for v1
const std::string CDecoder::m_tag_NGTF = "NGTF";  // Running period for v2
const std::string CDecoder::m_tag_EASF = "EASF";    // counter ...
const std::string CDecoder::m_tag_STGE = "STGE";    // Status 32 bits word
const std::string CDecoder::m_tag_EAIT = "EAIT";    // counter active energy injected to the network
const std::string CDecoder::m_tag_SINST1 = "SINST1";// apparent power for phase 1
const std::string CDecoder::m_tag_SINST2 = "SINST2";// apparent power for phase 2
const std::string CDecoder::m_tag_SINST3 = "SINST3";// apparent power for phase 3
const std::string CDecoder::m_tag_SINSTS = "SINSTS";// global apparent power
const std::string CDecoder::m_tag_UMOY1  = "UMOY1"; // mean voltage for phase 1
const std::string CDecoder::m_tag_UMOY2  = "UMOY2"; // mean voltage for phase 2
const std::string CDecoder::m_tag_UMOY3  = "UMOY3"; // mean voltage for phase 3

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
   :
   m_activeEnergyInjected(boost::make_shared<yApi::historization::CEnergy>("activeEnergyInjected")),
   m_runningPeriod(boost::make_shared<yApi::historization::CText>("runningPeriod")),
   m_runningIndex(boost::make_shared<yApi::historization::CCounter>("runningIndex")),
   m_tomorrowColor(boost::make_shared<linky::specificHistorizers::CColor>("tomorrowColor")),
   m_todayColor(boost::make_shared<linky::specificHistorizers::CColor>("todayColor")),
   m_api(api),
   m_deviceCreated(false),
   m_production(false),
   m_runningPeriodChanged(true),
   m_tomorrowColorChanged(true),
   m_todayColorChanged(true)
{
   m_activeIndex[0] = 0;
   m_activeIndex[1] = 0;

   for (int counter = 0; counter < 3; ++counter)
   {
      m_apparentPower[counter] = boost::make_shared<yApi::historization::CApparentPower>("apparentPowerPhase" + boost::lexical_cast<std::string>(counter + 1));
      m_meanVoltage[counter] = boost::make_shared<yApi::historization::CVoltage>("voltagePhase" + boost::lexical_cast<std::string>(counter + 1));
   }

   for (int counter = 0; counter < 10; ++counter)
      m_counter[counter] = boost::make_shared<yApi::historization::CEnergy>("Counter" + boost::lexical_cast<std::string>(counter+1));
}

CDecoder::~CDecoder()
{
}

void CDecoder::decodeLinkyMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages)
{
   bool triphases = (messages->find(m_tag_SINST2) != messages->end()) ? true : false;

   for (const auto message : *messages)
   {
      processMessage(message.first,
                     message.second);
   }

   if (!m_deviceCreated)
   {
      // Create all keywords
      createFirstKeywordList(triphases);
      createDeviceAndKeywords();
   }

   // Historizing only running keywords
   createRunningKeywordList(triphases);
   m_api->historizeData(m_deviceName, m_keywords);
}

void CDecoder::createDeviceAndKeywords()
{
   YADOMS_LOG(trace) << "Nb keywords : " << "=" << m_keywords.size() ;

   m_api->declareDevice(m_deviceName, "linkyUSB", 
                        "linkyUSB : Id = " + m_deviceName,
                        m_keywords,
                        m_DeviceDetails);

   m_deviceCreated = true;
}

void CDecoder::createFirstKeywordList(bool isTriphases)
{
   m_keywords.clear();

   m_keywords.push_back(m_runningPeriod);
   m_keywords.push_back(m_runningIndex);

   // We don't know in which contract we are, so we create theses keywords
   m_keywords.push_back(m_tomorrowColor);
   m_keywords.push_back(m_todayColor);

   // common for all contracts
   m_keywords.push_back(m_apparentPower[0]);
   m_keywords.push_back(m_meanVoltage[0]);

   for (unsigned char counter = 0; counter < 10; ++counter)
   {
      if (m_counter[counter]->get() != 0) // We register only counter != 0
         m_keywords.push_back(m_counter[counter]);
   }

   if (isTriphases) // We add missing phases
   {
      m_keywords.push_back(m_apparentPower[1]);
      m_keywords.push_back(m_apparentPower[2]);
      m_keywords.push_back(m_meanVoltage[1]);
      m_keywords.push_back(m_meanVoltage[2]);
   }

   if (m_production)
      m_keywords.push_back(m_activeEnergyInjected);
}

void CDecoder::createRunningKeywordList(bool isTriphases)
{
   static bool firstRun = true;
   m_keywords.clear();

   if (m_newPeriod != m_runningPeriod->get() || firstRun)
   {
      YADOMS_LOG(information) << m_runningPeriod;
      m_runningPeriod->set(m_newPeriod);
      m_keywords.push_back(m_runningIndex);
      m_keywords.push_back(m_runningPeriod);
   }

   if (m_todayColorChanged || firstRun)
      m_keywords.push_back(m_todayColor);

   if (m_tomorrowColorChanged || firstRun)
      m_keywords.push_back(m_tomorrowColor);

   m_keywords.push_back(m_apparentPower[0]);

   // At counter startup the value is 0
   // we historize only when the mean exists (0 during 10mn at counter power on)
   if (m_meanVoltage[0]->get()!=0)
      m_keywords.push_back(m_meanVoltage[0]);

   if (isTriphases) // We add missing phases
   {
      m_keywords.push_back(m_apparentPower[1]);
      m_keywords.push_back(m_apparentPower[2]);

      // At counter startup the value is 0
      // we historize only when the mean exists (0 during 10mn at counter power on)
      if (m_meanVoltage[1]->get() != 0)
         m_keywords.push_back(m_meanVoltage[1]);

      if (m_meanVoltage[2]->get() != 0)
         m_keywords.push_back(m_meanVoltage[2]);
   }

   if (firstRun)
   {
      for (unsigned char counter = 0; counter < 10; ++counter)
      {
         if (m_counter[counter]->get() != 0) // We register only counter != 0
            m_keywords.push_back(m_counter[counter]);
      }
   }
   else
   {
      // Historization of the active index only
      m_keywords.push_back(m_counter[m_activeIndex[0]]);

      // when the index changed, we register the old index also, to register the last index value
      if (m_activeIndex[0] != m_activeIndex[1])
         m_keywords.push_back(m_counter[m_activeIndex[1]]);
   }
   if (m_production)
      m_keywords.push_back(m_activeEnergyInjected);

   firstRun = false;
}

void CDecoder::processMessage(const std::string& key,
                              const std::vector<std::string>& values)
{
	try
	{
      //
      // tags LTARF and NGTF are not used to be the more generic as possible with all contracts of all suppliers

		if (key == m_tag_ADSC)
		{
			YADOMS_LOG(trace) << "ADSC" << "=" << values[0];

			static bool ADSCalreadyReceived = false;

			if (!ADSCalreadyReceived)
			{
				m_deviceName = values[0];
				ADSCalreadyReceived = true;
			}
		}
		else if (key == m_tag_VTIC)
		{
         YADOMS_LOG(trace) << "VTIC" << "=" << values[0];
         m_revision = boost::lexical_cast<int>(values[0]);
		}
		else if (key.find(m_tag_EASF)!= std::string::npos)
		{
			YADOMS_LOG(trace) << key << "=" << values[0];

         int counterIndex = boost::lexical_cast<int>(key.c_str()+4);
         m_counter[counterIndex - 1]->set(boost::lexical_cast<unsigned long>(values[0]));
		}
      else if (key == m_tag_STGE)
      {
         uint32_t status = 0;

         YADOMS_LOG(trace) << "STGE" << "=" << values[0];

         // Convert Hexa string to value
         std::stringstream ss;
         ss << std::hex << values[0];
         ss >> status;

         m_activeIndex[1] = m_activeIndex[0];
         m_activeIndex[0] = (status & 0x00003C00) >> 10; // bits 10 to 13

         m_runningIndex->set(m_activeIndex[0]);

         if (status & 0x00000100) // bit 8
            m_production = true;
         else
            m_production = false;

         linky::specificHistorizers::EColor temp = linky::specificHistorizers::EColor((status >> 24) & 0x03);
         if (temp != m_todayColor->get())
         {
            m_todayColorChanged = true;
            m_todayColor->set(temp); // bit 24 to 25 color of today
         }
         else
            m_todayColorChanged = false;

         temp = linky::specificHistorizers::EColor((status & 0x0C000000) >> 26);
         if (temp != m_tomorrowColor->get())
         {
            m_tomorrowColorChanged = true;
            m_tomorrowColor->set(temp); // bits 26 to 27 color of tomorrow
         }
         else
            m_tomorrowColorChanged = false;
      }
      else if (key == m_tag_EAIT)
      {
         YADOMS_LOG(trace) << "EAIT" << "=" << values[0];
         m_activeEnergyInjected->set(boost::lexical_cast<unsigned long>(values[0]));
      }
      else if (key == m_tag_SINST2)
      {
         YADOMS_LOG(trace) << "SINST2" << "=" << values[0];
         m_apparentPower[1]->set(boost::lexical_cast<double>(values[0]));
      }
      else if (key == m_tag_SINST3)
      {
         YADOMS_LOG(trace) << "STINST3" << "=" << values[0];
         m_apparentPower[2]->set(boost::lexical_cast<double>(values[0]));
      }
      else if (key == m_tag_UMOY1)
      {
         YADOMS_LOG(trace) << "UMOY1" << "=" << values[1];
         m_meanVoltage[0]->set(boost::lexical_cast<double>(values[1]));
      }
      else if (key == m_tag_UMOY2)
      {
         YADOMS_LOG(trace) << "UMOY2" << "=" << values[1];
         m_meanVoltage[1]->set(boost::lexical_cast<double>(values[1]));
      }
      else if (key == m_tag_UMOY3)
      {
         YADOMS_LOG(trace) << "UMOY3" << "=" << values[1];
         m_meanVoltage[2]->set(boost::lexical_cast<double>(values[1]));
      }
      else if (m_revision == 1) // specific functions v1
      {
         if (key == m_tag_SINST1)
         {
            m_apparentPower[0]->set(boost::lexical_cast<double>(values[0]));
         }
         else if (key == m_tag_LTARF) // For v1 running period is reverse with the NGTF tag !!
         {
            YADOMS_LOG(trace) << "LTARF" << "=<" << values[0] << ">";
            std::string value = values[0];

            m_newPeriod = trim(value);
         }
      }
      else if (m_revision == 2) // specific functions v2
      {
         YADOMS_LOG(trace) << "SINSTS" << "=" << values[0];
         if (key == m_tag_SINSTS)
         {
            m_apparentPower[0]->set(boost::lexical_cast<double>(values[0]));
         }
         else if (key == m_tag_NGTF)
         {
            YADOMS_LOG(trace) << "NGTF" << "= <" << values[0] << ">";
            std::string value = values[0];

            m_newPeriod = trim(value);
         }
      }
		else
		{
			YADOMS_LOG(trace) << "label is " << key << " not processed" ;
		}
	}
	catch (std::exception& e)
	{
		YADOMS_LOG(error) << "Exception received !" << e.what() ;
	}
}
