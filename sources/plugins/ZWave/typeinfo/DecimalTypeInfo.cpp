#include "stdafx.h"
#include "DecimalTypeInfo.h"
#include "../OpenZWaveHelpers.h"

CDecimalTypeInfo::CDecimalTypeInfo(OpenZWave::ValueID& vID)
   :m_data(shared::CDataContainer::make())
{
   initialize(vID);
}

CDecimalTypeInfo::~CDecimalTypeInfo()
{
}

boost::shared_ptr<shared::CDataContainer> CDecimalTypeInfo::serialize() const
{
   return m_data;
}

void CDecimalTypeInfo::initialize(OpenZWave::ValueID& vID)
{
   //extract values from OpenZWave data
   std::string name;
   std::string description;
   int min;
   int max;
   std::string unit;
   int precision;
   COpenZWaveHelpers::GetDecimalValueInfo(vID, name, description, min, max, unit, precision);

   //fill container
   m_data->set("name", name);
   m_data->set("description", description);
   m_data->set("unit", unit);
   m_data->set("min", min);
   m_data->set("max", max);
   m_data->set("precision", precision);


}

