#pragma once

#include <shared/plugin/yadomsApi/IYadomsApi.h>

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
      ///\param [in] type        The keyword type
      ///\param [in] units       The units
      ///\param [in] details     A free string which give units, enumeration values,....
      //------------------------------------
      CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::IYadomsApi::EKeywordType type, const std::string & units, const std::string& details);

      //------------------------------------
      ///\brief Constructor
      ///\param [in] keyword     The keyword used in Xpl data (i.e. : "temp")
      ///\param [in] capacity    The capacity implemented by the keyword (i.e. : "temperature")
      ///\param [in] accessMode  The acces mode (read and/or write)
      ///\param [in] type        The keyword type
      ///\param [in] units       The units
      ///\param [in] details     A free PropertyString which give units, enumeration values,....
      //------------------------------------
      CDeviceKeyword(const std::string& keyword, const std::string& capacity, shared::plugin::yadomsApi::IYadomsApi::EKeywordAccessMode accessMode, shared::plugin::yadomsApi::IYadomsApi::EKeywordType type, const std::string & units, const shared::CDataContainer & details);

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
      ///\brief Get keyword type
      ///\return keyword type
      //------------------------------------
      shared::plugin::yadomsApi::IYadomsApi::EKeywordType getType();

      //------------------------------------
      ///\brief Get keyword units
      ///\return keyword units
      //------------------------------------
      const std::string & getUnits();

      //------------------------------------
      ///\brief Get keyword details
      ///\return keyword details
      //------------------------------------
      const shared::CDataContainer & getDetails();      
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
      ///\brief The keyword type
      //------------------------------------
      shared::plugin::yadomsApi::IYadomsApi::EKeywordType m_type;

      //------------------------------------
      ///\brief The keyword units
      //------------------------------------
      const std::string& m_units;

      //------------------------------------
      ///\brief The keyword details
      //------------------------------------
      shared::CDataContainer m_details;
   };


} //namespace xplrules


