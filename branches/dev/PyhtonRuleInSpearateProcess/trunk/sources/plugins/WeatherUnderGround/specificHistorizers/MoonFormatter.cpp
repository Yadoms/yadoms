#include "stdafx.h"
#include "MoonFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>


namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

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
   CDataContainer Temp;

   Temp.set ("Units", m_Units);
   Temp.set ("IlluminatedMoon", m_IlluminatedMoon);
   Temp.set ("DayOfMoon", m_DayOfMoon);

   return Temp.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

