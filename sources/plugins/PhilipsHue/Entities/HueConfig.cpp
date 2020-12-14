#include "HueConfig.h"

CHueConfig::CHueConfig(const std::string& archetype, const std::string& function, const std::string& direction,
                       const CHueStartUp& startUp)
   : m_archetype(archetype),
     m_function(function),
     m_direction(direction),
     m_startUp(startUp)
{
}

const std::string& CHueConfig::getArchetype() const
{
   return m_archetype;
}

void CHueConfig::setArchetype(const std::string& archetype)
{
   m_archetype = archetype;
}

const std::string& CHueConfig::getFunction() const
{
   return m_function;
}

void CHueConfig::setFunction(const std::string& function)
{
   m_function = function;
}

const std::string& CHueConfig::getDirection() const
{
   return m_direction;
}

void CHueConfig::setDirection(const std::string& direction)
{
   m_direction = direction;
}

const CHueStartUp& CHueConfig::getStartUp() const
{
   return m_startUp;
}

void CHueConfig::setStartUp(const CHueStartUp& startUp)
{
   m_startUp = startUp;
}
