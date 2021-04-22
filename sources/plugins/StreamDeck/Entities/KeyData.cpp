#include "KeyData.h"

const std::string& CKeyData::getImg() const
{
   return m_img;
}

void CKeyData::setImg(const std::string& img)
{
   m_img = img;
}

const std::string& CKeyData::getCustomText() const
{
   return m_customText;
}

void CKeyData::setCustomText(const std::string& customText)
{
   m_customText = customText;
}

bool CKeyData::isIsAlreadyPressed() const
{
   return m_isAlreadyPressed;
}

void CKeyData::setIsAlreadyPressed(bool isAlreadyPressed)
{
   m_isAlreadyPressed = isAlreadyPressed;
}
