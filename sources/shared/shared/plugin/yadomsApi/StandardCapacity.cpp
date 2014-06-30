#include "stdafx.h"
#include "StandardCapacity.h"


namespace shared {   namespace plugin {      namespace yadomsApi      {

   CStandardCapacity::CStandardCapacity(const std::string & name, const std::string & unit, const EKeywordAccessMode accessMode, const EKeywordType type)
      :m_name(name), m_unit(unit), m_accessMode(accessMode), m_type(type)
   {

   }

   CStandardCapacity::~CStandardCapacity()
   {

   }

   const std::string & CStandardCapacity::getName() const
   {
      return m_name;
   }

   const std::string & CStandardCapacity::getUnit() const
   {
      return m_unit;
   }

   const EKeywordAccessMode CStandardCapacity::getAccessMode() const
   {
      return m_accessMode;
   }

   const EKeywordType CStandardCapacity::getType() const
   {
      return m_type;
   }

   CStandardCapacity::operator std::string() const
   {
      return getName();
   }


} } } // namespace shared::plugin::yadomsApi

