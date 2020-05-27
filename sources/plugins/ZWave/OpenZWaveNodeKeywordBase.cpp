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

std::string COpenZWaveNodeKeywordBase::getUnit() const
{
   return OpenZWave::Manager::Get()->GetValueUnits(m_valueId);
}



boost::shared_ptr<shared::CDataContainer> COpenZWaveNodeKeywordBase::serialize()
{
   return shared::CDataContainer::make();
}

boost::shared_ptr<shared::plugin::yPluginApi::typeInfo::ITypeInfo> & COpenZWaveNodeKeywordBase::getTypeInformation()
{
   return m_typeInformation;
}

bool COpenZWaveNodeKeywordBase::pressButton() const
{
   try
   {
      return OpenZWave::Manager::Get()->PressButton(m_valueId);
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send PressButton command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send PressButton command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send PressButton command : unknown exception";
   }
   return false;
}

bool COpenZWaveNodeKeywordBase::releaseButton() const
{
   try
   {
      return OpenZWave::Manager::Get()->ReleaseButton(m_valueId);
   }
   catch (OpenZWave::OZWException& ex)
   {
      YADOMS_LOG(error) << "Fail to send ReleaseButton command : OpenZWave exception : " << ex.what();
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send ReleaseButton command : std::exception : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send ReleaseButton command : unknown exception";
   }
   return false;
}

int COpenZWaveNodeKeywordBase::getIndex()
{
	return m_valueId.GetIndex();
}

ECommandClass COpenZWaveNodeKeywordBase::getCommandClass() 
{
	return ECommandClass(static_cast<int>(m_valueId.GetCommandClassId()));
}
