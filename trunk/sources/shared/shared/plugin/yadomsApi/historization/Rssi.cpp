#include "stdafx.h"
#include "Rssi.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CRssi::CRssi(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_rssi(0), m_measureType(measureType)
{
}

CRssi::~CRssi()
{
}

const std::string& CRssi::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CRssi::getCapacity() const
{
   return CStandardCapacities::Rssi;
}

void CRssi::set(int rssi)
{
   m_rssi = Normalize(rssi);
}

const std::string CRssi::formatValue() const
{
   return boost::lexical_cast<std::string>(rssi());
}

int CRssi::Normalize(int rssi)
{
   if (rssi > 100)
      return 100;
   if (rssi < 0)
      return 0;
   return rssi;
}

int CRssi::rssi() const
{
   return m_rssi;
}

const EMeasureType& CRssi::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

