#include "stdafx.h"
#include "Sms.h"

CSms::CSms(const std::string& number, const std::string& content)
   :m_number(number), m_content(content)
{
}

CSms::CSms(const GSM_MultiSMSMessage& gammuSms)
{
   // Need to decode the UTF-16 used by Gammu

   // 4 bytes max per character for UTF-8
   static const size_t Utf8Size = (MAX(sizeof(gammuSms.SMS[0].Number), sizeof(gammuSms.SMS[0].Text)) + 1) * 4;
   char utf8[Utf8Size];

   DecodeUnicode(gammuSms.SMS[0].Number, utf8);
   m_number = std::string(utf8);

   m_content.clear();
   for (int smsPart = 0; smsPart < gammuSms.Number; ++smsPart)
   {
      DecodeUnicode(gammuSms.SMS[smsPart].Text, utf8);
      m_content.append(utf8);
   }
}

const std::string& CSms::getNumber() const
{
   return m_number;
}

const std::string& CSms::getContent() const
{
   return m_content;
}
