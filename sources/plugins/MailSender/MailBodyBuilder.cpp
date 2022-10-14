#include "stdafx.h"
#include "MailBodyBuilder.h"

#include <utility>

#include "shared/tools/Random.h"

CMailBodyBuilder::CMailBodyBuilder(std::string sender,
                                   std::vector<std::string> recipients)
   : m_sectionWord(shared::tools::CRandom::generateUUID()),
     m_sender(std::move(sender)),
     m_recipients(std::move(recipients))
{
}

IMailBodyBuilder& CMailBodyBuilder::setSubject(const std::string& subject)
{
   m_subject = subject;
   return *this;
}

IMailBodyBuilder& CMailBodyBuilder::setAsciiBody(const std::string& message)
{
   m_asciiMessage = message;
   return *this;
}

IMailBodyBuilder& CMailBodyBuilder::setHtmlBody(const std::string& message)
{
   m_htmlMessage = message;
   return *this;
}

IMailBodyBuilder& CMailBodyBuilder::setCarbonCopyRecipients(std::vector<std::string> ccRecipients)
{
   m_ccRecipients = ccRecipients;
   return *this;
}


std::string CMailBodyBuilder::buildAsciiSection(const std::string& asciiText)
{
   std::string message = "Content-type: text/plain; charset=UTF-8\r\n";
   message += "\r\n";
   message += asciiText;
   message += "\r\n";
   return message;
}

std::string CMailBodyBuilder::buildHtmlSection(const std::string& htmlText)
{
   std::string message = "Content-type: text/html; charset=UTF-8\r\n";
   message += "\r\n";
   message += htmlText;
   message += "\r\n";
   return message;
}

std::string CMailBodyBuilder::build()
{
   std::string builtBody = "Mime-Version: 1.0\r\n";

   builtBody += "from: automatique<" + m_sender + ">\r\n";

   for (const auto& recipient : m_recipients)
      builtBody += "to: " + recipient + "\r\n";

   builtBody += "subject: " + m_subject + "\r\n";

   if (!m_asciiMessage.empty() && m_htmlMessage.empty())
   {
      builtBody += buildAsciiSection(m_asciiMessage);
   }
   else if (m_asciiMessage.empty() && !m_htmlMessage.empty())
   {
      builtBody += buildHtmlSection(m_htmlMessage);
   }
   else if (!m_asciiMessage.empty() && !m_htmlMessage.empty())
   {
      // Multi part message
      builtBody += "Content-Type: multipart/alternative; boundary=" + m_sectionWord + "\r\n";
      builtBody += "\r\n";

      // Ascii message section
      builtBody += "\r\n--" + m_sectionWord + "\r\n";
      builtBody += buildAsciiSection(m_asciiMessage);

      // Html message section
      builtBody += "--" + m_sectionWord + "\r\n";
      builtBody += buildHtmlSection(m_htmlMessage);

      builtBody += "\r\n--" + m_sectionWord + "--\r\n";
   }

   return builtBody;
}

