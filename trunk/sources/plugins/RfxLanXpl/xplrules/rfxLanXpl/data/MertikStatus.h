#pragma once

#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The mertik Status values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EMertikStatus,
      ((On))
      ((Off))
      ((StepUp))
      ((StepDown))
      ((RunUp))
      ((RunDown))
      ((Stop))
   );

   //-----------------------------------------------------
   ///\brief A mertick historizable object
   //-----------------------------------------------------
   class CMertickStatus : public shared::plugin::yadomsApi::historization::CSingleHistorizableData<EMertikStatus>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CMertickStatus(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMertickStatus();

   };



} } } // namespace xplrules::rfxLanXpl::data

