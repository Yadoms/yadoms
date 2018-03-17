#include "stdafx.h"
#include "Decoder.h"
#include <shared/Log.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
   s.erase(0, s.find_first_not_of(t));
   return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
   s.erase(s.find_last_not_of(t) + 1);
   return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
   return ltrim(rtrim(s, t), t);
}

const std::string CDecoder::m_tag_Config = "C";    // meter id

static const std::string m_tag_Up = "U";		// Buttun UP of the IO Controler
static const std::string m_tag_Down= "D";		// Buttun DOWN of the IO Controler
static const std::string m_tag_My = "M";		// Buttun MY of the IO Controler
static const std::string m_tag_Channel = "L";	// Buttun Channel of the IO Controler
static const std::string m_tag_Prog= "P";		// Buttun Prog of the IO Controler
static const std::string m_tag_Read = "R";		// Read Buffer
static const std::string m_tag_Write = "W";		// Write Buffer
static const std::string m_tag_Status = "S";	// Get the version of the IO Controler Adapter card
static const std::string m_tag_Error = "E";		// An error occured

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
   :
   m_runningPeriod(boost::make_shared<yApi::historization::CText>("runningPeriod")),
   m_runningIndex(boost::make_shared<yApi::historization::CCounter>("runningIndex")),
   m_api(api),
   m_deviceCreated(false),
   m_activeChannel(0)
{
}

CDecoder::~CDecoder()
{
}

void CDecoder::decodeSomfyIOControlerMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const boost::shared_ptr<std::map<std::string, std::vector<std::string>>>& messages)
{
   for (const auto message : *messages)
   {
      processMessage(message.first,
                     message.second);
   }

   if (!m_deviceCreated)
   {
      // Create all keywords
      createDeviceAndKeywords();
	  createFirstKeywordList();
      m_api->historizeData(m_deviceName, m_keywords);
   }
   else {
      // Historizing only running keywords
      m_api->historizeData(m_deviceName, m_keywords);
   }
}

void CDecoder::createDeviceAndKeywords()
{
   YADOMS_LOG(trace) << "Nb keywords : " << "=" << m_keywords.size() ;

   m_api->declareDevice(m_deviceName, "SomfyIOControlerUSB", 
                        "SomfyIOControlerUSB : Id = " + m_deviceName,
                        m_keywords,
                        m_DeviceDetails);

   m_deviceCreated = true;
}

void CDecoder::createFirstKeywordList()
{
   m_keywords.clear();

   m_runningPeriod->set(m_newPeriod);
   m_keywords.push_back(m_runningPeriod);
   m_keywords.push_back(m_runningIndex);

}

void CDecoder::createRunningKeywordList()
{
   m_keywords.clear();

   if (m_newPeriod != m_runningPeriod->get())
   {
      m_runningPeriod->set(m_newPeriod);
      m_keywords.push_back(m_runningIndex);
      m_keywords.push_back(m_runningPeriod);
   }

   // Historization of the active index only
  // m_keywords.push_back(m_counter[m_activeChannel]);

}

void CDecoder::processMessage(const std::string& key,
                              const std::vector<std::string>& values)
{
	try
	{
		static const std::string m_tag_Config;
		static const std::string m_tag_Up;
		static const std::string m_tag_Down;
		static const std::string m_tag_My;
		static const std::string m_tag_Channel;
		static const std::string m_tag_Prog;
		static const std::string m_tag_Read;
		static const std::string m_tag_Write;
		static const std::string m_tag_Status;
		static const std::string m_tag_Error;
		if (key == m_tag_Config)
		{
			YADOMS_LOG(trace) << "Config" << "=" << values[0];
		}
		else if (key == m_tag_Up)
		{
			YADOMS_LOG(trace) << "Up" << "=" << values[0];
		}
		else if (key == m_tag_Down)
		{
			YADOMS_LOG(trace) << "Down" << "=" << values[0];
		}
		else if (key == m_tag_My)
		{
			YADOMS_LOG(trace) << "My" << "=" << values[0];
		}
		else if (key == m_tag_Channel)
		{
			YADOMS_LOG(trace) << "Channel" << "=" << values[0];
		}

		else if (key == m_tag_Prog)
		{
			YADOMS_LOG(trace) << "Prog" << "=" << values[0];
		}

		else if (key == m_tag_Read)
		{
			YADOMS_LOG(trace) << "Read" << "=" << values[0];
		}
		else if (key == m_tag_Write)
		{
			YADOMS_LOG(trace) << "Write" << "=" << values[0];
		}
		else if (key == m_tag_Error)
		{
			YADOMS_LOG(trace) << "Error" << "=" << values[0];
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
