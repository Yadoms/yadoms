#pragma once
#include "UTERequest.h"


namespace message::radioErp1
{
	//--------------------------------------------------------------
	/// \brief	GigaConcept specific UTE message
	///
	/// Code very-specific for a bugged device : the remote control socket DO21-12B-E from Giga-Concept.
	/// This device send the userData (and only this part of the message) in reversed byte order.
	/// If we recognize this message, we have to reorder bytes
	//--------------------------------------------------------------
	class CUTEGigaConceptReversedRequest final : public CUTERequest
	{
	public:
		static bool isCGigaConceptReversedUteMessage(const CReceivedMessage& message);

		//--------------------------------------------------------------
		/// \brief	                           Constructor
		/// \param[in] message                 The ERP1 radio message
		//--------------------------------------------------------------
		explicit CUTEGigaConceptReversedRequest(const CReceivedMessage& message);
		~CUTEGigaConceptReversedRequest() override = default;

		bool bidirectionalCommunication() const override;
		bool teachInResponseExpected() const override;
		ETeachInRequest teachInRequest() const override;
		unsigned char command() const override;
		unsigned char channelNumber() const override;
		unsigned int manufacturerId() const override;
		unsigned char type() const override;
		unsigned char func() const override;
		unsigned char rorg() const override;
	};
} // namespace message
