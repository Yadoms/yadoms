#pragma once
#include "IGeneralInfo.h"
#include "IDayLight.h"
#include "ILocation.h"
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../dataAccessLayer/IConfigurationManager.h"

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
      ((ConnectedGuiClientsCount))
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
      //-----------------------------------------------------
      CGeneralInfo(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CGeneralInfo();

   protected:
      // IGeneralInfo Implementation
      virtual std::string get(const std::string& key) const;
      // [END] IGeneralInfo Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Location helper
      //-----------------------------------------------------
      boost::shared_ptr<ILocation> m_location;

      //-----------------------------------------------------
      ///\brief               Day light helper
      //-----------------------------------------------------
      boost::shared_ptr<IDayLight> m_dayLight;
   };

} } // namespace automation::script
	
	