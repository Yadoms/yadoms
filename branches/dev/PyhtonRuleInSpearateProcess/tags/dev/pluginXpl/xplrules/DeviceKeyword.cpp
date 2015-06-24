#include "stdafx.h"
#include "DeviceKeyword.h"
#include <shared/serialization/PTreeToJsonSerializer.h>

namespace xplrules {

   CDeviceKeyword::CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::IYadomsApi::EKeywordType type, const std::string & units, const std::string& details)
      :m_keyword(keyword), m_capacity(capacity), m_accessMode(accessMode), m_type(type), m_units(units), m_details(details)
   {
   }

   CDeviceKeyword::CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::IYadomsApi::EKeywordType type, const std::string & units, const boost::property_tree::ptree & details)
      : m_keyword(keyword), m_capacity(capacity), m_accessMode(accessMode), m_type(type), m_units(units)
   {
      shared::serialization::CPtreeToJsonSerializer detailsSerialiser;
      m_details = detailsSerialiser.serialize(details);
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

   shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode CDeviceKeyword::getAccessMode()
   {
      return m_accessMode;
   }

   shared::plugin::yadomsApi::IYadomsApi::EKeywordType CDeviceKeyword::getType()
   {
      return m_type;
   }

   const std::string & CDeviceKeyword::getUnits()
   {
      return m_units;
   }

   const std::string & CDeviceKeyword::getDetails()
   {
      return m_details;
   }

} //namespace xplrules


