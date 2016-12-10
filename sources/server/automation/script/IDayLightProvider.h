#pragma once

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The day light interface
      //-----------------------------------------------------
      class IDayLightProvider
      {
      public:
         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~IDayLightProvider()
         {
         }

         //-----------------------------------------------------
         ///\brief               Compute sunrise time
         ///\return Sunrise hour
         //-----------------------------------------------------
         virtual boost::posix_time::ptime sunriseTime() = 0;

         //-----------------------------------------------------
         ///\brief               Compute sunset time
         ///\return Sunset hour
         //-----------------------------------------------------
         virtual boost::posix_time::ptime sunsetTime() = 0;
      };
   }
} // namespace automation::script


