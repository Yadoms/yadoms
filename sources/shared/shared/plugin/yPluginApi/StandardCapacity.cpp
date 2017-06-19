#include "stdafx.h"
#include "StandardCapacity.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         CStandardCapacity::CStandardCapacity(const std::string& name, const std::string& unit, const EKeywordDataType& type)
            : m_name(name), m_unit(unit), m_type(type)
         {
         }

         CStandardCapacity::~CStandardCapacity()
         {
         }

         const std::string& CStandardCapacity::getName() const
         {
            return m_name;
         }

         const std::string& CStandardCapacity::getUnit() const
         {
            return m_unit;
         }

         const EKeywordDataType& CStandardCapacity::getType() const
         {
            return m_type;
         }

         CStandardCapacity::operator const std::string &() const
         {
            return getName();
         }
      }
   }
} // namespace shared::plugin::yPluginApi


