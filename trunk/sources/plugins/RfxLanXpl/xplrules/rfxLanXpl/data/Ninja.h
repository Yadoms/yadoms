#pragma once

#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(ENinjaCommand,
      ((On))
      ((Off))
      ((StepUp))
      ((StepDown))
      ((RunUp))
      ((RunDown))
      ((Stop))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CNinja : public shared::plugin::yadomsApi::historization::CSingleHistorizableData<ENinjaCommand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CNinja(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CNinja();

   };



} } } // namespace xplrules::rfxLanXpl::data

