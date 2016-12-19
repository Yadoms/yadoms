#include "stdafx.h"
#include "CameraMove.h"
#include "../StandardValues.h"


namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            DECLARE_ENUM_IMPLEMENTATION(ECameraMoveCommand,
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
            );

            CCameraMove::CCameraMove(const std::string& keywordName,
                                     const EKeywordAccessMode& accessMode)
               : CSingleHistorizableData<ECameraMoveCommand>(keywordName,
                                                             CStandardCapacities::CameraMove(),
                                                             accessMode,
                                                             ECameraMoveCommand::kCenterPosition)
            {
            }

            CCameraMove::~CCameraMove()
            {
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


