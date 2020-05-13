#include "stdafx.h"
#include "OrangeBusinessConfiguration.h"

COrangeBusinessConfiguration::COrangeBusinessConfiguration()
{
}

COrangeBusinessConfiguration::~COrangeBusinessConfiguration()
{
}

void COrangeBusinessConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data.initializeWith(data);
}

const std::string COrangeBusinessConfiguration::getAPIKey() const
{
   return m_data.get<std::string>("APIKey");
}