#pragma once

#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>


namespace rfxcomMessages { namespace specificHistorizers {


   //-----------------------------------------------------
   ///\brief               The remote codes
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(ERemoteAtiWonder2TypesCodes,
      ((PC)(0))
      ((AUX1)(1))
      ((AUX2)(2))
      ((AUX3)(3))
      ((AUX4)(4))
   );
   
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CRemoteAtiWonder2CmdTypeHistorizer : public shared::plugin::yadomsApi::historization::CSingleHistorizableData<ERemoteAtiWonder2TypesCodes>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CRemoteAtiWonder2CmdTypeHistorizer(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CRemoteAtiWonder2CmdTypeHistorizer();

   };



} } // namespace rfxcomMessages::specificHistorizers

