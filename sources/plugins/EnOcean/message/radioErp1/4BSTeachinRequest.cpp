#include "stdafx.h"
#include "4BSTeachinRequest.h"
#include "profiles/bitsetHelpers.hpp"

using namespace message::radioErp1;

C4BSTeachinRequest::C4BSTeachinRequest(const boost::dynamic_bitset<>& data)
	: m_data(data)
{
}

C4BSTeachinRequest::~C4BSTeachinRequest()
{
}

unsigned int C4BSTeachinRequest::manufacturerId() const
{
	return bitset_extract(m_data, 13, 11);
}

C4BSTelegram::EFuncIds C4BSTeachinRequest::funcId() const
{
	return C4BSTelegram::toFuncId(bitset_extract(m_data, 0, 6));
}

unsigned int C4BSTeachinRequest::typeId() const
{
	return bitset_extract(m_data, 6, 7);
}

