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
         ///\brief   Get the update location
         ///\return  The update location
         //---------------------------------------------
         virtual const std::string getUpdateLocation() const = 0;

         //---------------------------------------------
         ///\brief   Get the current installed version
         ///\return  The current installed version
         //---------------------------------------------
         virtual const tools::CVersion getInstalledVersion() const = 0;

         //---------------------------------------------
         ///\brief   Get the type of update (main app, plugin, widget,...)
         ///\return  The type of update
         //---------------------------------------------
         virtual const EUpdateType getUpdateType() const = 0;

      };

   } // namespace source
} // namespace update