#include "stdafx.h"
#include "MoonFormatter.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CMoonFormatter::CMoonFormatter( )
{}

void CMoonFormatter::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_Units.set ( UnitName, UnitValue );
}

void CMoonFormatter::SetParameters(
            const std::string& IlluminatedMoon,
            const std::string& DayofMoon
				)
{
   m_IlluminatedMoon = IlluminatedMoon;
   m_DayOfMoon = DayofMoon;
}

CMoonFormatter::~CMoonFormatter()
{}

std::string CMoonFormatter::formatValue() const
{
   shared::CDataContainer Temp;

   Temp.set ("Units", m_Units);
   Temp.set ("IlluminatedMoon", m_IlluminatedMoon);
   Temp.set ("DayOfMoon", m_DayOfMoon);

   return Temp.serialize();
}