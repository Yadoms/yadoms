#include "stdafx.h"
#include "OpenZWaveNodeKeywordBase.h"

COpenZWaveNodeKeywordBase::COpenZWaveNodeKeywordBase(OpenZWave::ValueID & valueId)
   :m_valueId(valueId)
{
}

COpenZWaveNodeKeywordBase::~COpenZWaveNodeKeywordBase()
{
}   
   
void COpenZWaveNodeKeywordBase::updateValue(OpenZWave::ValueID & value)
{
   m_valueId = value;
}

const std::string COpenZWaveNodeKeywordBase::getUnit()
{
   return OpenZWave::Manager::Get()->GetValueUnits(m_valueId);
}