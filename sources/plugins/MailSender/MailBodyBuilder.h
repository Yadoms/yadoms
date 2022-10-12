#pragma once
#include <vector>

#include "IMailBodyBuilder.h"


//--------------------------------------------------------------
/// \brief	This class build the email body
//--------------------------------------------------------------
class CMailBodyBuilder final : public IMailBodyBuilder
{
public:
   /// @brief Constructor
   /// @param sender Sender address
   /// @param recipients Recipient addresses, as a list of alias<email> strings (alias is optional)
   CMailBodyBuilder(std::string sender,
                    std::vector<std::string> recipients);
   CMailBodyBuilder(const CMailBodyBuilder&) = delete;
   CMailBodyBuilder(const CMailBodyBuilder&&) = delete;
   CMailBodyBuilder operator=(const CMailBodyBuilder&) = delete;
   CMailBodyBuilder operator=(const CMailBodyBuilder&&) = delete;
   ~CMailBodyBuilder() override = default;

   // IMailBodyBuilder implementation
   IMailBodyBuilder& setSubject(const std::string& subject) override;
   IMailBodyBuilder& setAsciiBody(const std::string& message) override;
   IMailBodyBuilder& setHtmlBody(const std::string& message) override;
   IMailBodyBuilder& setCarbonCopyRecipients(std::vector<std::string> ccRecipients) override;
   std::string build() override;
   // [END] IMailBodyBuilder implementation

private:
   static std::string buildAsciiSection(const std::string& asciiText);
   static std::string buildHtmlSection(const std::string& htmlText);

   const std::string m_sectionWord;

   const std::string m_sender;
   const std::vector<std::string> m_recipients;
   std::string m_subject;
   std::string m_asciiMessage;
   std::string m_htmlMessage;
   std::vector<std::string> m_ccRecipients;

   std::string m_builtBody;
};
