#include "stdafx.h"
#include "Sms.h"

CSms::CSms(const std::string& number, const std::string& content)
   :m_number(number), m_content(content)
{
}

CSms::CSms(const GSM_MultiSMSMessage& gammuSms)
{
   char strUtf8[MAX(sizeof(gammuSms.SMS[0].Number), sizeof(gammuSms.SMS[0].Text)) * 2];
   EncodeUTF8(strUtf8, gammuSms.SMS[0].Number);//TODO conversion KO
   m_number = std::string(strUtf8);

   m_content.clear();
   for (int smsPart = 0 ; smsPart < gammuSms.Number ; ++ smsPart)
   {
      EncodeUTF8(strUtf8, gammuSms.SMS[smsPart].Text);
      m_content.append(boost::lexical_cast<std::string>(strUtf8));
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
