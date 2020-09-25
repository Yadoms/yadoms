#pragma once
#include "ICurrentTime.h"

namespace shared
{
   namespace currentTime
   {
      //--------------------------------------------------------------
      /// \brief	   The current time provider interface
      //--------------------------------------------------------------
      class CProvider
      {
      public:
         //--------------------------------------------------------------
         /// \brief	   Constructor (initialize the static object)
         /// \param[in] timeInstance Time to use as provider
         //--------------------------------------------------------------
         explicit CProvider(boost::shared_ptr<ICurrentTime> timeInstance);

         //--------------------------------------------------------------
         /// \brief	   Destructor
         //--------------------------------------------------------------
         virtual ~CProvider() = default;

         //--------------------------------------------------------------
         /// \brief	   Set time provider instance
         /// \param[in] timeInstance The time instance to used
         /// \return Now
         //--------------------------------------------------------------
         void setProvider(boost::shared_ptr<ICurrentTime> timeInstance);

         //--------------------------------------------------------------
         /// \brief	   Returns now time (local time)
         /// \return Now
         //--------------------------------------------------------------
         boost::posix_time::ptime now();

      private:
         //--------------------------------------------------------------
         /// \brief	   The currentTime object
         //--------------------------------------------------------------
         boost::shared_ptr<ICurrentTime> m_currentTimeInstance;
      };

      // The time provider instance
      CProvider& Provider();
   }
} // namespace shared::currentTime


