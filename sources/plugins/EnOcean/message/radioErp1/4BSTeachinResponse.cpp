#include "stdafx.h"
#include "4BSTeachinResponse.h"
#include "profiles/bitsetHelpers.hpp"
#include <boost/dynamic_bitset.hpp>

using namespace message::radioErp1;

C4BSTeachinResponse::C4BSTeachinResponse(const std::string& senderId,
										 const std::string& destinationId,
										 unsigned char status,
										 C4BSTelegram::EFuncIds funcId,
										 unsigned int typeId,
										 unsigned int manufacturerId)
	: CSendMessage(CRorgs::ERorgIds::k4BS_Telegram,
				   senderId,
				   destinationId,
				   status),
	m_funcId(funcId),
	m_typeId(typeId),
	m_manufacturerId(manufacturerId)
{
	boost::dynamic_bitset<> userData(4 * 8);

	bitset_insert(userData, 0, 6, m_funcId);
	bitset_insert(userData, 6, 7, m_typeId);
	bitset_insert(userData, 13, 11, m_manufacturerId);

	bitset_insert(userData, 24, 1, true); // LRN Type : telegram with EEP number and Manufacturer ID
	bitset_insert(userData, 25, 1, true); // EEP : EEP supported
	bitset_insert(userData, 26, 1, true); // LRN Result : Sender ID stored
	bitset_insert(userData, 27, 1, true); // LRN Status : Response
	bitset_insert(userData, 28, 1, false); // LRN Bit : Teach-in telegram

	CSendMessage::userData(bitset_to_bytes(userData));
}

C4BSTeachinResponse::~C4BSTeachinResponse()
{
}

