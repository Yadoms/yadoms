#include "stdafx.h"
#include "Voltage.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CVoltage::CVoltage(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_voltage(0.0), m_measureType(measureType)
{
}

CVoltage::~CVoltage()
{
}

const std::string& CVoltage::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CVoltage::getCapacity() const
{
   return CStandardCapacities::Voltage;
}

void CVoltage::set(const shared::CDataContainer& yadomsCommand)
{
   m_voltage = yadomsCommand.get<double>("voltage");
}

void CVoltage::set(double voltage)
{
   m_voltage = voltage;
}

const std::string CVoltage::formatValue() const
{
   return boost::lexical_cast<std::string>(voltage());
}

double CVoltage::voltage() const
{
   return m_voltage;
}

const EMeasureType& CVoltage::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

