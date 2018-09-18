#include "stdafx.h"
#include "OpenZWaveNodeKeywordBase.h"
#include "typeinfo/TypeInfoFactory.h"

COpenZWaveNodeKeywordBase::COpenZWaveNodeKeywordBase(OpenZWave::ValueID& valueId)
   : m_valueId(valueId), m_typeInformation(CTypeInfoFactory::create(valueId))
{
}

COpenZWaveNodeKeywordBase::~COpenZWaveNodeKeywordBase()
{
}

void COpenZWaveNodeKeywordBase::updateValue(OpenZWave::ValueID& value)
{
   m_valueId = value;
}

const std::string COpenZWaveNodeKeywordBase::getUnit()
{
   return OpenZWave::Manager::Get()->GetValueUnits(m_valueId);
}



shared::CDataContainer COpenZWaveNodeKeywordBase::serialize()
{
   shared::CDataContainer result;
   return result;
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::typeInfo::ITypeInfo> & COpenZWaveNodeKeywordBase::getTypeInformation()
{
   return m_typeInformation;
}

