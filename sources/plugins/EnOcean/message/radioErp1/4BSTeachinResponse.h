#pragma once
#include "SendMessage.h"

namespace message::radioErp1
{
	class C4BSTeachinResponse : public CSendMessage
	{
	public:
		explicit C4BSTeachinResponse(const std::string& senderId,
									 const std::string& destinationId,
									 unsigned char status,
									 C4BSTelegram::EFuncIds funcId,
									 unsigned int typeId,
									 unsigned int manufacturerId);
		virtual ~C4BSTeachinResponse();

	private:
		const C4BSTelegram::EFuncIds m_funcId;
		const unsigned int m_typeId;
		const unsigned int m_manufacturerId;
	};
}