#pragma once

namespace shared
{
   namespace currentTime
   {
      //--------------------------------------------------------------
      /// \brief	   The current time provider interface
      //--------------------------------------------------------------
      class ICurrentTime
      {
      public:
         virtual ~ICurrentTime()
         {
         }

         //--------------------------------------------------------------
         /// \brief	   Get the current time
         /// \return    The current time
         //--------------------------------------------------------------
         virtual boost::posix_time::ptime now() const = 0;
      };
   }
} // namespace shared::currentTime


