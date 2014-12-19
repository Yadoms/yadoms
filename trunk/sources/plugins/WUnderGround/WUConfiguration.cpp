#include "stdafx.h"
#include "WUConfiguration.h"
#include <shared/StringExtension.h>

CWUConfiguration::~CWUConfiguration()
{
}

void CWUConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);
}

std::string CWUConfiguration::getAPIKey() const
{
   return m_data.get<std::string>("APIKey");
}

std::string CWUConfiguration::getLocalisation() const
{
   return m_data.get<std::string>("Localisation");
}