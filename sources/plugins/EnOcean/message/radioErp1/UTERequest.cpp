#include "stdafx.h"
#include "UTERequest.h"


using namespace message::radioErp1;

CUTERequest::CUTERequest(const radioErp1::CReceivedMessage& message)
	: m_erp1(message)
{
}

bool CUTERequest::bidirectionalCommunication() const
{
	return (m_erp1.userData()[0] & 0x80) ? true : false;
}

bool CUTERequest::teachInResponseExpected() const
{
	return (m_erp1.userData()[0] & 0x40) ? false : true;
}

CUTERequest::ETeachInRequest CUTERequest::teachInRequest() const
{
	return static_cast<ETeachInRequest>((m_erp1.userData()[0] & 0x30) >> 4);
}

unsigned char CUTERequest::command() const
{
	return m_erp1.userData()[0] & 0x0F;
}

unsigned char CUTERequest::channelNumber() const
{
	return m_erp1.userData()[1];
}

unsigned int CUTERequest::manufacturerId() const
{
	return m_erp1.userData()[2] | ((m_erp1.userData()[3] & 0x07) << 8);
}

unsigned char CUTERequest::type() const
{
	return m_erp1.userData()[4];
}

unsigned char CUTERequest::func() const
{
	return m_erp1.userData()[5];
}

unsigned char CUTERequest::rorg() const
{
	return m_erp1.userData()[6];
}

std::string CUTERequest::senderId() const
{
	return m_erp1.senderId();
}
