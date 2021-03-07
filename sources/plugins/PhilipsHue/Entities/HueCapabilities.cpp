#include "HueCapabilities.h"

CHueCapabilities::CHueCapabilities(bool& certified, const CHueControl& control, const CHueStreaming& streaming)
   : m_certified(certified),
     m_control(control),
     m_streaming(streaming)
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

const CHueStreaming& CHueCapabilities::getStreaming() const
{
   return m_streaming;
}

void CHueCapabilities::setStreaming(CHueStreaming& streaming)
{
   m_streaming = streaming;
}
