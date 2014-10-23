#pragma once

#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yadomsApi/historization/SingleHistorizableData.hpp>

namespace xplrules { namespace rfxLanXpl { namespace data {

   //-----------------------------------------------------
   ///\brief               The mertik command values
   //-----------------------------------------------------   
   DECLARE_ENUM_HEADER(EMertikCommand,
      ((Left))
      ((Right))
      ((Up))
      ((Down))
      ((P1))
      ((ProgP1))
      ((P2))
      ((ProgP2))
      ((P3))
      ((ProgP3))
      ((P4))
      ((ProgP4))
      ((Center))
      ((ProgCenter))
      ((Sweep))
      ((ProgSweep))
   );

   //-----------------------------------------------------
   ///\brief A mertick historizable object
   //-----------------------------------------------------
   class CMertickCommand : public shared::plugin::yadomsApi::historization::CSingleHistorizableData<EMertikCommand>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CMertickCommand(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMertickCommand();

   };



} } } // namespace xplrules::rfxLanXpl::data

