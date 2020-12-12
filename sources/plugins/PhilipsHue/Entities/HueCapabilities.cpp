#include "HueCapabilities.h"

CHueCapabilities::CHueCapabilities(bool& certified, const CHueControl& control)
   : m_certified(certified),
     m_control(control)
{
}

const bool& CHueCapabilities::isCertified() const
{
   return m_certified;
}

void CHueCapabilities::setCertified(bool certified)
{
   m_certified = certified;
}

const CHueControl& CHueCapabilities::getControl() const
{
   return m_control;
}

void CHueCapabilities::setControl(CHueControl control)
{
   m_control = control;
}
