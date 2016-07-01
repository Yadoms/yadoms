#include "stdafx.h"
#include "OpenZWaveEnumHandlerTypeInfo.h"
#include "OpenZWaveEnumHandler.h"

COpenZWaveEnumHandlerTypeInfo::COpenZWaveEnumHandlerTypeInfo(OpenZWave::ValueID& vID)
   : m_data(getTypeInfo(vID))
{
}

COpenZWaveEnumHandlerTypeInfo::~COpenZWaveEnumHandlerTypeInfo()
{
}

shared::CDataContainer COpenZWaveEnumHandlerTypeInfo::serialize() const
{
   return m_data;
}

shared::CDataContainer COpenZWaveEnumHandlerTypeInfo::getTypeInfo(OpenZWave::ValueID& vID)
{
   COpenZWaveEnumHandler value(vID);
   shared::CDataContainer result;
   result.set("name", value.getName());
   result.set("values", value.getAllStrings());
   return result;
}

