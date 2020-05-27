#include "stdafx.h"
#include "StringTypeInfo.h"
#include "../OpenZWaveHelpers.h"

CStringTypeInfo::CStringTypeInfo(OpenZWave::ValueID& vID)
   :m_data(shared::CDataContainer::make())
{
   initialize(vID);
}

CStringTypeInfo::~CStringTypeInfo()
{
}

boost::shared_ptr<shared::CDataContainer> CStringTypeInfo::serialize() const
{
   return m_data;
}

void CStringTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   shared::CDataContainer result;
   std::string name;
   std::string description;
   COpenZWaveHelpers::GetStringValueInfo(vID, name, description);

   m_data->set("name", name);
   m_data->set("description", description);
}

