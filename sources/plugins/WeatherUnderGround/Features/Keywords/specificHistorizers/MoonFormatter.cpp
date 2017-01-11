#include "stdafx.h"
#include "MoonFormatter.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizer
{
   CMoonFormatter::CMoonFormatter()
   {
   }

   void CMoonFormatter::addUnit(const std::string& unitName,
                                const std::string& unitValue
   )
   {
      m_units.set(unitName,
                  unitValue);
   }

   void CMoonFormatter::setParameters(const std::string& illuminatedMoon,
                                      const std::string& dayofMoon)
   {
      m_illuminatedMoon = illuminatedMoon;
      m_dayOfMoon = dayofMoon;
   }

   CMoonFormatter::~CMoonFormatter()
   {
   }

   std::string CMoonFormatter::formatValue() const
   {
      shared::CDataContainer temp;

      temp.set("Units", m_units);
      temp.set("IlluminatedMoon", m_illuminatedMoon);
      temp.set("DayOfMoon", m_dayOfMoon);

      return temp.serialize();
   }
}