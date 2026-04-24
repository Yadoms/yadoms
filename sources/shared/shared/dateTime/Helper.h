#pragma once

namespace shared
{
   namespace dateTime
   {
      //--------------------------------------------------------------
      /// \brief	DateTime helper functions
      //--------------------------------------------------------------
      class CHelper
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Get next rounded minute
         /// \param[in] refTime Reference time
         /// \return	the next minute
         //--------------------------------------------------------------
         static boost::posix_time::ptime nextMinuteOf(const boost::posix_time::ptime& refTime);
      };
   }
} //namespace shared::dateTime
