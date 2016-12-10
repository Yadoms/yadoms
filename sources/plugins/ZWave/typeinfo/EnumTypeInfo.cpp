#include "stdafx.h"
#include "EnumTypeInfo.h"
#include "../OpenZWaveHelpers.h"

CEnumTypeInfo::CEnumTypeInfo(OpenZWave::ValueID& vID)
{
   initialize(vID);
}

CEnumTypeInfo::~CEnumTypeInfo()
{
}

shared::CDataContainer CEnumTypeInfo::serialize() const
{
   return m_data;
}

void CEnumTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   std::string name;
   std::string description;
   std::map<int, std::string> values;
   COpenZWaveHelpers::GetEnumValueInfo(vID, name, description, values);

   m_data.set("name", name);
   m_data.set("description", description);
   
   shared::CDataContainer resultValues;
   for (auto i = values.begin(); i != values.end(); ++i)
      resultValues.set( (boost::format("%1%") % i->first).str(), i->second);

   m_data.set("values", resultValues);

   std::cout << m_data.serialize() << std::endl;
}



