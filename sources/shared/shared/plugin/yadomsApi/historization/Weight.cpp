#include "stdafx.h"
#include "Weight.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CWeight::CWeight(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_weight(0.0), m_measureType(measureType)
{
}

CWeight::~CWeight()
{
}

const std::string& CWeight::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CWeight::getCapacity() const
{
   return CStandardCapacities::Weight;
}

void CWeight::set(const shared::CDataContainer& yadomsCommand)
{
   m_weight = yadomsCommand.get<double>("weight");
}

void CWeight::set(double weight)
{
   m_weight = weight;
}

const std::string CWeight::formatValue() const
{
   return boost::lexical_cast<std::string>(weight());
}

double CWeight::weight() const
{
   return m_weight;
}

const EMeasureType& CWeight::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

