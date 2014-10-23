#pragma once

#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The ninja-robocam command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EUvDescription,
      ((Low))
      ((Medium))
      ((High))
      ((VeryHigh))
      ((Dangerous))
   );

   //-----------------------------------------------------
   ///\brief A Ninja historizable object
   //-----------------------------------------------------
   class CUvDescription : public shared::plugin::yadomsApi::historization::CSingleHistorizableData<EUvDescription>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CUvDescription(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CUvDescription();

   };



} } } // namespace xplrules::rfxLanXpl::data

