#include "HueSwUpdate.h"

CHueSwUpdate::CHueSwUpdate(const std::string& state, const std::string& lastInstall)
   : m_state(state),
     m_lastInstall(lastInstall)
{
}

const std::string& CHueSwUpdate::getState() const
{
   return m_state;
}

void CHueSwUpdate::setState(std::string state)
{
   m_state = state;
}

const std::string& CHueSwUpdate::getLastInstall() const
{
   return m_lastInstall;
}

void CHueSwUpdate::setLastInstall(std::string lastInstall)
{
   m_lastInstall = lastInstall;
}
