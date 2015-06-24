#include "stdafx.h"
#include "DeviceKeyword.h"

namespace xplrules {

   CDeviceKeyword::CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::EKeywordType type, const std::string & units, const std::string& details)
      :m_keyword(keyword), m_capacity(capacity), m_accessMode(accessMode), m_type(type), m_units(units), m_details(details)
   {
   }

   CDeviceKeyword::CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::EKeywordType type, const std::string & units, const shared::CDataContainer & details)
      : m_keyword(keyword), m_capacity(capacity), m_accessMode(accessMode), m_type(type), m_units(units), m_details(details)
   {
   }


   CDeviceKeyword::~CDeviceKeyword()
   {
   }

   const std::string & CDeviceKeyword::getName()
   {
      return m_keyword;
   }

   const std::string & CDeviceKeyword::getCapacity()
   {
      return m_capacity;
   }

   shared::plugin::yadomsApi::EKeywordAccessMode CDeviceKeyword::getAccessMode()
   {
      return m_accessMode;
   }

   shared::plugin::yadomsApi::EKeywordType CDeviceKeyword::getType()
   {
      return m_type;
   }

   const std::string & CDeviceKeyword::getUnits()
   {
      return m_units;
   }

   const shared::CDataContainer & CDeviceKeyword::getDetails()
   {
      return m_details;
   }

} //namespace xplrules


