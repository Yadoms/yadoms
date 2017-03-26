#include "stdafx.h"
#include "Dongle.h"
#include <Poco/RegularExpression.h>

CDongle::CDongle()
{
}
   
CDongle::~CDongle()
{
}

boost::shared_ptr<CDongle> CDongle::create(const std::string & helloCommandAnswer)
{
   boost::shared_ptr<CDongle> dongle = boost::shared_ptr<CDongle>();

   //Welcome to Ziblue Dongle RFPLAYER (RFP1000, Firmware V1.12 Mac 0xF6C09FCC)
   const std::string regex1_11 = "Welcome to Ziblue Dongle (?<type>[^,\\ ]*) \\((?<model>[^,]*), Firmware V(?<firmwareVersion>\\d{1,}\\.\\d{1,})(?<fullmac> Mac (?<mac>0x[0-9A-fa-f]{8}))?\\)";

   Poco::RegularExpression re(regex1_11);
   std::vector<std::string> results;
   int resultCount = re.split(helloCommandAnswer, results);

   if (resultCount > 3)
   {
      dongle = boost::shared_ptr<CDongle>(new CDongle()); //dont use boost_make_shared, because ctor is private

      //result[0] is the regex, not used
      dongle->m_type = results[1];
      dongle->m_model = results[2];
      dongle->m_firmwareVersion = results[3];

      //optionnaly, mac address (not given by all firmwares)
      if(results.size()>5)
         dongle->m_macAddress = results[5];
   }
   return dongle;
}

const std::string & CDongle::getType() const
{
   return m_type;
}

const std::string & CDongle::getModel() const
{
   return m_model;
}

const std::string & CDongle::getFirmwareVersion() const
{
   return m_firmwareVersion;
}

const std::string & CDongle::getMacAddress() const
{
   return m_macAddress;
}

