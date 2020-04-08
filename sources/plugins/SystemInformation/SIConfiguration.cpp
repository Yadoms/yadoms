#include "stdafx.h"
#include "SIConfiguration.h"
#include <shared/Log.h>

CSIConfiguration::CSIConfiguration()
   :m_data(new_CDataContainerSharedPtr())
{
}

CSIConfiguration::~CSIConfiguration()
{
}

void CSIConfiguration::initializeWith(const shared::CDataContainerSharedPtr& data)
{
   m_data->initializeWith(data);
}