#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include "SingleHistorizableData.hpp"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief               The curtain command values
            //-----------------------------------------------------   
            DECLARE_ENUM_HEADER_SHARED(ECameraMoveCommand, YADOMS_SHARED_EXPORT,
               ((Left))
               ((Right))
               ((Up))
               ((Down))
               ((Position1))
               ((ProgramPosition1))
               ((Position2))
               ((ProgramPosition2))
               ((Position3))
               ((ProgramPosition3))
               ((Position4))
               ((ProgramPosition4))
               ((CenterPosition))
               ((ProgramCenterPosition))
               ((Sweep))
               ((ProgramSweep))
            ) ;


            //-----------------------------------------------------
            ///\brief A curtain historizable object
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CCameraMove : public CSingleHistorizableData<ECameraMoveCommand>
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CCameraMove(const std::string& keywordName,
                                    const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet,
                                    const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CCameraMove() = default;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
