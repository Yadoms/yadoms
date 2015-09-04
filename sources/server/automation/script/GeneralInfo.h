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
      virtual std::string get(shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const;
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

      //-----------------------------------------------------
      ///\brief               Running information provider
      //-----------------------------------------------------
      boost::shared_ptr<IRunningInformation> m_runningInformation;
   };

} } // namespace automation::script
	
	