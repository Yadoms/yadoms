#include "stdafx.h"
#include "LoadKeyword.h"
#include <shared/plugin/yadomsApi/StandardCapacity.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLoadKeyword::CLoadKeyword(const std::string& keywordName)
   :m_keywordName(keywordName), m_load(0.0)
{
}

CLoadKeyword::~CLoadKeyword()
{
}

const std::string& CLoadKeyword::getKeyword() const
{
   return m_keywordName;
}

const yApi::CStandardCapacity& CLoadKeyword::getCapacity() const
{
   static const yApi::CStandardCapacity LoadCapacity("load", yApi::CStandardUnits::Percent, yApi::EKeywordAccessMode::kGet, yApi::EKeywordDataType::kNumeric);
   return LoadCapacity;
}

void CLoadKeyword::set(float load)
{
   m_load = load;
}

const std::string CLoadKeyword::formatValue() const
{
   std::ostringstream ss;
   ss << std::fixed << std::setprecision(2) << load();
   return ss.str();
}

float CLoadKeyword::load() const
{
   return m_load;
}

const yApi::historization::EMeasureType& CLoadKeyword::getMeasureType() const
{
   static const yApi::historization::EMeasureType MeasureType(yApi::historization::EMeasureType::kAbsolute);
   return MeasureType;
}

