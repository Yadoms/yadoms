#include "stdafx.h"
#include "OledEspEasyConfiguration.h"
#include <shared/Log.h>

COledEspEasyConfiguration::~COledEspEasyConfiguration()
{
}

void COledEspEasyConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_configuration.initializeWith(data);
}

/*
    You will find here an example how to retrieve enum variables from the configuration
*/

int COledEspEasyConfiguration::getOledLineCount() const
{
	if(m_configuration.exists("specificOledSize.checkbox") && m_configuration.get<bool>("specificOledSize.checkbox"))
	{
		return m_configuration.get<int>("specificOledSize.content.LineCount");
	}
	return 8;
}

std::string COledEspEasyConfiguration::getIPAddress() const
{
	return m_configuration.get<std::string>("ipAddress");
}



void COledEspEasyConfiguration::trace() const
{
   try
   {
      // Get simple parameters
	   YADOMS_LOG(information) << "OledEspEasy configuration, parameter 'ipAddress' is " << (getIPAddress().empty() ? "empty" : getIPAddress());
        ;
      // Enum
      YADOMS_LOG(information) << "OledEspEasy configuration, parameter 'Line count' is " << getOledLineCount();
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