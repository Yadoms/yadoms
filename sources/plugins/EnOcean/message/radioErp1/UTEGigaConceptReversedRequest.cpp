#include "stdafx.h"
#include "UTEGigaConceptReversedRequest.h"


using namespace message::radioErp1;

bool CUTEGigaConceptReversedRequest::isCGigaConceptReversedUteMessage(const CReceivedMessage& message)
{
	// Try to recognize if userData is in revert order, only for device known as bugged.
	if (const auto userData = message.userData(); userData.size() == 7
		&& userData[0] == 0xD2 // Profile - Rorg
		&& userData[1] == 0x01 // Profile - Func
		&& userData[2] == 0x08 // Profile - Type
		&& userData[3] == 0x00 // Manufacturer ID MSB
		&& userData[4] == 0x3E // Manufacturer ID LSB ==> GigaConceptFrance
		&& userData[5] == 0xFF)
		return true;

	// Normal message
	return false;
}

CUTEGigaConceptReversedRequest::CUTEGigaConceptReversedRequest(const CReceivedMessage& message)
	: CUTERequest(message)
{
}

bool CUTEGigaConceptReversedRequest::bidirectionalCommunication() const
{
	return (m_erp1.userData()[6] & 0x80) ? true : false;
}

bool CUTEGigaConceptReversedRequest::teachInResponseExpected() const
{
	return (m_erp1.userData()[6] & 0x40) ? false : true;
}

CUTEGigaConceptReversedRequest::ETeachInRequest CUTEGigaConceptReversedRequest::teachInRequest() const
{
	return static_cast<ETeachInRequest>((m_erp1.userData()[6] & 0x30) >> 4);
}

unsigned char CUTEGigaConceptReversedRequest::command() const
{
	return m_erp1.userData()[6] & 0x0F;
}

unsigned char CUTEGigaConceptReversedRequest::channelNumber() const
{
	return m_erp1.userData()[5];
}

unsigned int CUTEGigaConceptReversedRequest::manufacturerId() const
{
	return m_erp1.userData()[4] | ((m_erp1.userData()[3] & 0x07) << 8);
}

unsigned char CUTEGigaConceptReversedRequest::type() const
{
	return m_erp1.userData()[2];
}

unsigned char CUTEGigaConceptReversedRequest::func() const
{
	return m_erp1.userData()[1];
}

unsigned char CUTEGigaConceptReversedRequest::rorg() const
{
	return m_erp1.userData()[0];
}

