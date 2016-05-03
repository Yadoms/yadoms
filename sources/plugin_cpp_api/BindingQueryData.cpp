#include "stdafx.h"
#include "BindingQueryData.h"


CBindingQueryData::CBindingQueryData(const std::string& query)
   :m_query(query)
{
}

CBindingQueryData::~CBindingQueryData()
{
}

const std::string& CBindingQueryData::getQuery() const
{
   return m_query;
}