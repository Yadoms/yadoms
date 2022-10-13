#pragma once

#include "IPendingMail.h"


//--------------------------------------------------------------
/// \brief	This class stores a pending mail
//--------------------------------------------------------------
class CPendingMail final : public IPendingMail
{
public:
   /// @brief Constructor
   /// @param recipientId The recipient ID
   /// @param body The body to send
   CPendingMail(int recipientId,
                std::string body);
   CPendingMail(const CPendingMail&) = delete;
   CPendingMail(const CPendingMail&&) = delete;
   CPendingMail operator=(const CPendingMail&) = delete;
   CPendingMail operator=(const CPendingMail&&) = delete;
   ~CPendingMail() override = default;

   // IPendingMail implementation
   int recipientId() override;
   std::string body() override;
   bool retry() override;
   // [END] IPendingMail implementation

private:
   const int m_recipientId;
   const std::string m_body;
   int m_tries = 5;
};
