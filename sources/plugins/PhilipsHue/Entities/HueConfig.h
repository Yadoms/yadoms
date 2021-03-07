#pragma once
#include "HueStartUp.h"

class CHueConfig
{
public:

   CHueConfig() = default;

   CHueConfig(const std::string& archetype, const std::string& function, const std::string& direction,
              const CHueStartUp& startUp);
   virtual ~CHueConfig() = default;

   const std::string& getArchetype() const;
   void setArchetype(const std::string& archetype);
   const std::string& getFunction() const;
   void setFunction(const std::string& function);
   const std::string& getDirection() const;
   void setDirection(const std::string& direction);
   const CHueStartUp& getStartUp() const;
   void setStartUp(const CHueStartUp& startUp);

private:
   std::string m_archetype;
   std::string m_function;
   std::string m_direction;
   CHueStartUp m_startUp;
};
