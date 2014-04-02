#include "stdafx.h"
#include "Sms.h"

CSms::CSms(const std::string& number, const std::string& content)
   :m_number(number), m_content(content)
{
}

const std::string& CSms::getNumber() const
{
   return m_number;
}

const std::string& CSms::getContent() const
{
   return m_content;
}
