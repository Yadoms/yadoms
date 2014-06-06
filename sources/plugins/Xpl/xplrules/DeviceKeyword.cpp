#include "stdafx.h"
#include "DeviceKeyword.h"

namespace xplrules {

   CDeviceKeyword::CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, const std::string& details)
      :m_keyword(keyword), m_capacity(capacity), m_accessMode(accessMode), m_details(details)
   {
   }

   CDeviceKeyword::~CDeviceKeyword()
   {
   }

   //------------------------------------
   ///\brief Get keyword name
   ///\return keyword name
   //------------------------------------
   const std::string & CDeviceKeyword::getName()
   {
      return m_keyword;
   }

   //------------------------------------
   ///\brief Get keyword capacity
   ///\return keyword capacity
   //------------------------------------
   const std::string & CDeviceKeyword::getCapacity()
   {
      return m_capacity;
   }

   //------------------------------------
   ///\brief Get keyword capacity mode
   ///\return keyword capacity mode
   //------------------------------------
   shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode CDeviceKeyword::getAccessMode()
   {
      return m_accessMode;
   }

   //------------------------------------
   ///\brief Get keyword details
   ///\return keyword details
   //------------------------------------
   const std::string & CDeviceKeyword::getDetails()
   {
      return m_details;
   }

} //namespace xplrules


