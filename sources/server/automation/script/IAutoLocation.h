#pragma once
#include <shared/DataContainer.h>

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The auto-location interface
      //-----------------------------------------------------
      class IAutoLocation
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IAutoLocation()
         {
         }

         //-----------------------------------------------------
         ///\brief               Try to auto locate
         /// \return the acquired location
         /// \throw shared::exception::CEmptyResult if fails
         /// \note returned CDataContainer must provide these fields :
         ///     - latitude (as double)
         ///     - longitude (as double)
         ///     - altitude (as double)
         //-----------------------------------------------------
         virtual shared::CDataContainer tryAutoLocate() const = 0;
      };
   }
} // namespace automation::script


