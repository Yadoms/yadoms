#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_configuration.initializeWith(data);
}


std::string CConfiguration::getIPAddress() const
{
	return m_configuration.get<std::string>("ipAddress");
}

EScreenType CConfiguration::getSceenType() const
{
   if (m_configuration.exists("screenSpecs.content.screenType"))
   {

      // Enum type, declare keys labels
      static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
         ("TFT", kTft)
         ("EINK", kEInk);

      return m_configuration.getEnumValue<EScreenType>("screenSpecs.content.screenType", EEnumTypeNames);
   }
   return kTft;
}

//--------------------------------------------------------------
/// \brief	    Obtain the screen width in pixels
//--------------------------------------------------------------
unsigned int CConfiguration::getSceenWidth() const
{
   if (m_configuration.exists("screenSpecs.content.screenWidth"))
   {
      return m_configuration.get<int>("screenSpecs.content.screenWidth");
   }
   return 250;
}
//--------------------------------------------------------------
/// \brief	    Obtain the screen height in pixels
//--------------------------------------------------------------
unsigned int  CConfiguration::getSceenHeight() const
{
   if (m_configuration.exists("screenSpecs.content.screenHeight"))
   {
      return m_configuration.get<int>("screenSpecs.content.screenHeight");
   }
   return 250;
}


void CConfiguration::trace() const
{
   try
   {
      // Get simple parameters
	   YADOMS_LOG(information) << "HTTP Screen configuration, parameter 'ipAddress' is " << (getIPAddress().empty() ? "empty" : getIPAddress());
      YADOMS_LOG(information) << "HTTP Screen configuration, parameter 'Screen Type' (0: TFT, 1:eInk) is " << getSceenType();
      YADOMS_LOG(information) << "HTTP Screen configuration, parameter 'Screen Width' is " << getSceenWidth();
      YADOMS_LOG(information) << "HTTP Screen configuration, parameter 'Screen Height' is " << getSceenHeight();
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}