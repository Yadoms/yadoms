#include "stdafx.h"
#include "IntegerTypeInfo.h"
#include "../OpenZWaveHelpers.h"
#include <shared/Log.h>

CIntegerTypeInfo::CIntegerTypeInfo(OpenZWave::ValueID& vID)
{
   initialize(vID);
}

CIntegerTypeInfo::~CIntegerTypeInfo()
{
}

shared::CDataContainer CIntegerTypeInfo::serialize() const
{
   return m_data;
}

void CIntegerTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   std::string name;
   std::string description;
   int min;
   int max;
   std::string unit;
   COpenZWaveHelpers::GetIntegerValueInfo(vID, name, description, min, max, unit);

   m_data.set("name", name);
   m_data.set("description", description);
   m_data.set("unit", unit);
   m_data.set("min", min);
   m_data.set("max", max);
   m_data.set("step", 1);

   YADOMS_LOG(information) << m_data.serialize() ;
}

