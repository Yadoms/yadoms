#include "stdafx.h"
#include "Uv.h"
#include "../StandardValues.h"
#include "../StandardCapacities.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
{

CUv::CUv(const std::string& keywordName, const EMeasureType& measureType)
   :m_keywordName(keywordName), m_uv(0.0), m_measureType(measureType)
{
}

CUv::~CUv()
{
}

const std::string& CUv::getKeyword() const
{
   return m_keywordName;
}

const CStandardCapacity& CUv::getCapacity() const
{
   return CStandardCapacities::Uv;
}

void CUv::set(const shared::CDataContainer& yadomsCommand)
{
   m_uv = yadomsCommand.get<double>("uv");
}

void CUv::set(double uv)
{
   m_uv = uv;
}

const std::string CUv::formatValue() const
{
   return boost::lexical_cast<std::string>(uv());
}

double CUv::uv() const
{
   return m_uv;
}

const EMeasureType& CUv::getMeasureType() const
{
   return m_measureType;
}

} } } } // namespace shared::plugin::yadomsApi::historization

