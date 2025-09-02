#pragma once
#include "../Esp3SendPacket.h"
#include "profiles/eep.h"

namespace message::radioErp1
{
	//--------------------------------------------------------------
	/// \brief	EnOcean send command message
	//--------------------------------------------------------------
	class CSendMessage : public CEsp3SendPacket
	{
	public:
		explicit CSendMessage(CRorgs::ERorgIds rorg,
							  const std::string& senderId,
							  const std::string& destinationId,
							  unsigned char status);
		~CSendMessage() override = default;

		void userData(const std::vector<unsigned char>& userData);

		boost::shared_ptr<const std::vector<unsigned char>> buffer() override;

		std::string destinationId() const;

	private:
		CRorgs::ERorgIds m_rorg;
		const std::string m_senderId;
		const std::string m_destinationId;
		std::vector<unsigned char> m_userData;
		unsigned char m_status;
	};
} // namespace message
