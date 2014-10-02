#include "stdafx.h"
#include "Humidity.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{

CHumidity::CHumidity(const std::string& keywordName)
   :m_keywordName(keywordName), m_humidity(0)
{
}

CHumidity::~CHumidity()
{
}

const std::string& CHumidity::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CHumidity::getCapacity() const
{
   return CStandardCapacities::Humidity;
}

void CHumidity::set(const shared::CDataContainer& yadomsCommand)
{
   m_humidity = Normalize(yadomsCommand.get<int>("humidity"));
}

void CHumidity::set(int humidity)
{
   m_humidity = Normalize(humidity);
}

const std::string CHumidity::formatValue() const
{
   return boost::lexical_cast<std::string>(humidity());
}

int CHumidity::Normalize(int value)
{
   if (value > 100)
      return 100;
   if (value < 0)
      return 0;
   return value;
}

int CHumidity::humidity() const
{
   return m_humidity;
}

} } } } // namespace shared::plugin::yadomsApi::commands

