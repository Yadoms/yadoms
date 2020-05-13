#include "stdafx.h"
#include "SIConfiguration.h"
#include <shared/Log.h>

CSIConfiguration::CSIConfiguration()
   :m_data(shared::CDataContainer::make())
{
}

CSIConfiguration::~CSIConfiguration()
{
}

void CSIConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data->initializeWith(data);
}