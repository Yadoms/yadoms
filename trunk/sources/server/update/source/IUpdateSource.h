#pragma once

#include "tools/Version.h"
#include "../UpdateType.h"

namespace update {
   namespace source {

      //---------------------------------------------
      ///\brief Interface for UpdateSource
      ///       Provide a source of update
      //---------------------------------------------
      class IUpdateSource
      {
      public:
         //---------------------------------------------
         ///\brief Virtual destructor
         //---------------------------------------------
         virtual ~IUpdateSource() {};

         //---------------------------------------------
         ///\brief   Get the type of update (main app, plugin, widget,...)
         ///\return  The type of update
         //---------------------------------------------
         virtual const EUpdateType getUpdateType() const = 0;

      };

   } // namespace source
} // namespace update