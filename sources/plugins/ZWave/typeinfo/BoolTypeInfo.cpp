#include "stdafx.h"
#include "BoolTypeInfo.h"
#include "../OpenZWaveHelpers.h"

CBoolTypeInfo::CBoolTypeInfo(OpenZWave::ValueID& vID)
{
   initialize(vID);
}

CBoolTypeInfo::~CBoolTypeInfo()
{
}

shared::CDataContainer CBoolTypeInfo::serialize() const
{
   return m_data;
}

void CBoolTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   std::string name;
   std::string description;
   COpenZWaveHelpers::GetBooleanValueInfo(vID, name, description);

   m_data.set("name", name);
   m_data.set("description", description);
}

