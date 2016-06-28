#include "stdafx.h"
#include "SIConfiguration.h"

CSIConfiguration::CSIConfiguration()
{
}

CSIConfiguration::~CSIConfiguration()
{
}

void CSIConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_data.initializeWith(data);
}