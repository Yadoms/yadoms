#include "stdafx.h"
#include "WESConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CWESConfiguration::CWESConfiguration()
   :m_data(shared::CDataContainer::make())
{}

CWESConfiguration::~CWESConfiguration()
{}

void CWESConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data->initializeWith(data);
}

bool CWESConfiguration::isRetrieveNamesFromdevice() const
{
   return m_data->get<bool>("retrieveNamesFromdevice");
}