#pragma once

#include <shared/plugin/yadomsApi/IYadomsAPI.h>

namespace xplrules {

   //------------------------------------
   ///\brief Class container for device keyword information.
   //------------------------------------
   class CDeviceKeyword
   {
   public:
      //------------------------------------
      ///\brief Constructor
      ///\param [in] keyword     The keyword used in Xpl data (i.e. : "temp")
      ///\param [in] capacity    The capacity implemented by the keyword (i.e. : "temperature")
      ///\param [in] accessMode  The acces mode (read and/or write)
      ///\param [in] details     A free string which give units, enumeration values,....
      //------------------------------------
      CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, const std::string& details);

      //------------------------------------
      ///\brief Constructor
      ///\param [in] keyword     The keyword used in Xpl data (i.e. : "temp")
      ///\param [in] capacity    The capacity implemented by the keyword (i.e. : "temperature")
      ///\param [in] accessMode  The acces mode (read and/or write)
      ///\param [in] details     A free PropertyString which give units, enumeration values,....
      //------------------------------------
      CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, const boost::property_tree::ptree & details);

      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~CDeviceKeyword();

      //------------------------------------
      ///\brief Get keyword name
      ///\return keyword name
      //------------------------------------
      const std::string & getName();

      //------------------------------------
      ///\brief Get keyword capacity
      ///\return keyword capacity
      //------------------------------------
      const std::string & getCapacity();

      //------------------------------------
      ///\brief Get keyword capacity mode
      ///\return keyword capacity mode
      //------------------------------------
      shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode getAccessMode();

      //------------------------------------
      ///\brief Get keyword details
      ///\return keyword details
      //------------------------------------
      const std::string & getDetails();      
   private:
     
      //------------------------------------
      ///\brief The keyword name
      //------------------------------------
      const std::string& m_keyword;

      //------------------------------------
      ///\brief The keyword capacity
      //------------------------------------
      const std::string& m_capacity;

      //------------------------------------
      ///\brief The keyword capacity mode
      //------------------------------------
      shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode m_accessMode;

      //------------------------------------
      ///\brief The keyword details
      //------------------------------------
      std::string m_details;      
   };


} //namespace xplrules


