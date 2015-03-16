#pragma once
#include "IGeneralInfo.h"
#include "IDayLight.h"
#include "ILocation.h"
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../dataAccessLayer/IConfigurationManager.h"
#include "../../IRunningInformation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief Info type
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(EInfo,
      ((Sunrise))
      ((Sunset))

      ((Latitude))
      ((Longitude))
      ((Altitude))

      ((YadomsServerOS))
      ((YadomsServerVersion))
   )

   //-----------------------------------------------------
   ///\brief The script properties
   //-----------------------------------------------------
   class CGeneralInfo : public IGeneralInfo
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configurationManager  Configuration manager (to gain access to Yadoms configuration from rules scripts)
      ///\param[in] runningInformation  Running information provider
      //-----------------------------------------------------
      CGeneralInfo(
         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
         boost::shared_ptr<IRunningInformation> runningInformation);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CGeneralInfo();

   protected:
      // IGeneralInfo Implementation
      virtual std::string get(const std::string& key) const;
      // [END] IGeneralInfo Implementation


      //-----------------------------------------------------
      ///\brief               To string converter : used to convert data to string, locale-independently
      ///\param[in] value     Value to convert
      ///\return              Converted value, using the C locale
      //-----------------------------------------------------
      template<typename T>
      std::string toString(const T& value) const
      {
         std::ostringstream ss;
         ss.imbue(std::locale::classic()); // Use the C locale 
         ss << value;
         return ss.str();
      }

   private:
      //-----------------------------------------------------
      ///\brief               Location helper
      //-----------------------------------------------------
      boost::shared_ptr<ILocation> m_location;

      //-----------------------------------------------------
      ///\brief               Day light helper
      //-----------------------------------------------------
      boost::shared_ptr<IDayLight> m_dayLight;

      //-----------------------------------------------------
      ///\brief               Running information provider
      //-----------------------------------------------------
      boost::shared_ptr<IRunningInformation> m_runningInformation;
   };

} } // namespace automation::script
	
	