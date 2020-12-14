#include "HueControl.h"

CHueControl::CHueControl(int minDimLevel, int maxLumen, const std::string& colorGamutType,
                         const CHueColorGamut& colorGamut,
                         const CHueCt& ct)
   : m_minDimLevel(minDimLevel),
     m_maxLumen(maxLumen),
     m_colorGamutType(colorGamutType),
     m_colorGamut(colorGamut),
     m_ct(ct)
{
}

const int& CHueControl::getMinDimLevel() const
{
   return m_minDimLevel;
}

void CHueControl::setMinDimLevel(int minDimLevel)
{
   m_minDimLevel = minDimLevel;
}

const int& CHueControl::getMaxLumen() const
{
   return m_maxLumen;
}

void CHueControl::setMaxLumen(int maxLumen)
{
   m_maxLumen = maxLumen;
}

const std::string& CHueControl::getColorGamutType() const
{
   return m_colorGamutType;
}

void CHueControl::setColorGamutType(std::string colorGamutType)
{
   m_colorGamutType = colorGamutType;
}

const CHueColorGamut& CHueControl::getColorGamut() const
{
   return m_colorGamut;
}

void CHueControl::setColorGamut(CHueColorGamut& colorGamut)
{
   m_colorGamut = colorGamut;
}

const CHueCt& CHueControl::getCt() const
{
   return m_ct;
}

void CHueControl::setCt(CHueCt& ct)
{
   m_ct = ct;
}
