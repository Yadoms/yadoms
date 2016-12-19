#include "stdafx.h"
#include "ConditionHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

DECLARE_CAPACITY(ConditionCapacity, "Condition", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData);

CConditionHistorizer::CConditionHistorizer(const std::string& keywordName,
                                           const yApi::EKeywordAccessMode& accessMode)
   : m_keywordName(keywordName),
     m_accessMode(accessMode)
{
   m_content = boost::make_shared<CConditionFormatter>();
}

CConditionHistorizer::~CConditionHistorizer()
{
}

const std::string& CConditionHistorizer::getKeyword() const
{
   return m_keywordName;
}

const yApi::CStandardCapacity& CConditionHistorizer::getCapacity() const
{
   return ConditionCapacity();
}

const yApi::EKeywordAccessMode& CConditionHistorizer::getAccessMode() const
{
   return m_accessMode;
}

void CConditionHistorizer::addUnit(const std::string& unitName,
                                   const std::string& unitValue)
{
   m_content->addUnit(unitName, unitValue);
}

void CConditionHistorizer::setPeriod(const std::string& time,
                                     const std::string& weatherCondition,
                                     const std::string& temp,
                                     const std::string& pressure,
                                     const std::string& visibility,
                                     const std::string& uv,
                                     const std::string& dewPoint,
                                     const std::string& maxWind,
                                     const std::string& aveWind,
                                     const std::string& aveWindDegrees,
                                     const std::string& humidity,
                                     const std::string& rainDay,
                                     const std::string& feelslike,
                                     const std::string& windChill)
{
   m_content->setPeriod(time,
                        weatherCondition,
                        temp,
                        pressure,
                        visibility,
                        uv,
                        dewPoint,
                        maxWind,
                        aveWind,
                        aveWindDegrees,
                        humidity,
                        rainDay,
                        feelslike,
                        windChill);
}

std::string CConditionHistorizer::formatValue() const
{
   if (!m_content)
      return std::string();

   return m_content->formatValue();
}

const yApi::historization::EMeasureType& CConditionHistorizer::getMeasureType() const
{
   static const auto MeasureType(yApi::historization::EMeasureType::kAbsolute);
   return MeasureType;
}

shared::CDataContainer CConditionHistorizer::getTypeInfo() const
{
   return shared::CDataContainer();
}

void CConditionHistorizer::setCityName(const std::string& cityName)
{
   m_content->setCityName(cityName);
}

