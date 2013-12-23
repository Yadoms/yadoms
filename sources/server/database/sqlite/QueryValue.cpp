#include "stdafx.h"
#include "QueryValue.h"


CQueryValue::CQueryValue() 
   :m_bIsDefined(false)
{ 
}

CQueryValue::~CQueryValue() 
{
}

CQueryValue::CQueryValue(const std::string & value) 
{ 
   Initialize("'" + value + "'");
}

CQueryValue::CQueryValue(const bool value) 
{ 
   Initialize(std::string(value?"1":"0"));
}

CQueryValue& CQueryValue::operator=(const CQueryValue & anyValue)
{
   Initialize(anyValue.m_valueAsString);
   return *this;
}

const std::string & CQueryValue::str() const
{
   return m_valueAsString;
}



const bool CQueryValue::IsDefined() const 
{ 
   return m_bIsDefined;
}

void CQueryValue::Initialize(const std::string & value)
{
   m_valueAsString = value; 
   m_bIsDefined = true; 
}

