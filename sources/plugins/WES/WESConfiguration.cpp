#include "stdafx.h"
#include "WESConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CWESConfiguration::CWESConfiguration()
{}

CWESConfiguration::~CWESConfiguration()
{}

void CWESConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}