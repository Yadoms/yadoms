#include "stdafx.h"
#include "EnumTypeInfo.h"
#include "../OpenZWaveHelpers.h"
#include <shared/Log.h>

CEnumTypeInfo::CEnumTypeInfo(OpenZWave::ValueID& vID)
   :m_data(new_CDataContainerSharedPtr())
{
   initialize(vID);
}

CEnumTypeInfo::~CEnumTypeInfo()
{
}

shared::CDataContainerSharedPtr CEnumTypeInfo::serialize() const
{
   return m_data;
}

void CEnumTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   std::string name;
   std::string description;
   std::map<int, std::string> values;
   COpenZWaveHelpers::GetEnumValueInfo(vID, name, description, values);

   m_data->set("name", name);
   m_data->set("description", description);
   
   shared::CDataContainer resultValues;
   for (auto i = values.begin(); i != values.end(); ++i)
      resultValues.set( (boost::format("%1%") % i->first).str(), i->second);

   m_data->set("values", resultValues);

   YADOMS_LOG(information) << m_data->serialize() ;
}



