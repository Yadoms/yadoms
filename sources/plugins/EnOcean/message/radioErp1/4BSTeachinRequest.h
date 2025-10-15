#pragma once
#include "manufacturers.h"
#include "profiles/eep.h"

namespace message::radioErp1
{
	class C4BSTeachinRequest
	{
	public:
		explicit C4BSTeachinRequest(const boost::dynamic_bitset<>& data);
		virtual ~C4BSTeachinRequest();

		unsigned int manufacturerId() const;
		C4BSTelegram::EFuncIds funcId() const;
		unsigned int typeId() const;

	private:
		const boost::dynamic_bitset<>& m_data;
	};
}