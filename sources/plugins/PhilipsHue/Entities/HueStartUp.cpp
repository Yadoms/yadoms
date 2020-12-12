#include "HueStartUp.h"

CHueStartUp::CHueStartUp(const std::string& mode, bool& configured)
   : m_mode(mode),
     m_configured(configured)
{
}

const std::string& CHueStartUp::getMode() const
{
   return m_mode;
}

void CHueStartUp::setMode(const std::string& mode)
{
   m_mode = mode;
}

bool CHueStartUp::isConfigured() const
{
   return m_configured;
}

void CHueStartUp::setConfigured(bool& configured)
{
   m_configured = configured;
}
