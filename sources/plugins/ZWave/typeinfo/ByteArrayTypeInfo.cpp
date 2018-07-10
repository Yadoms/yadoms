#include "stdafx.h"
#include "ByteArrayTypeInfo.h"
#include "../OpenZWaveHelpers.h"
#include <shared/Log.h>

CByteArrayTypeInfo::CByteArrayTypeInfo(OpenZWave::ValueID& vID)
{
   initialize(vID);
}

CByteArrayTypeInfo::~CByteArrayTypeInfo()
{
}

shared::CDataContainer CByteArrayTypeInfo::serialize() const
{
   return m_data;
}

void CByteArrayTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   std::string name;
   std::string description;
   COpenZWaveHelpers::GetStringValueInfo(vID, name, description);

   m_data.set("name", name);
   m_data.set("description", description);
   YADOMS_LOG(information) << m_data.serialize() ;
}

