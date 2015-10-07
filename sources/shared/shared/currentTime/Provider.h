#pragma once
#include "ICurrentTime.h"
#include <shared/Export.h>

namespace shared { namespace currentTime
{
   //--------------------------------------------------------------
   /// \brief	   The current time provider interface
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT Provider
   {
   public:
      //--------------------------------------------------------------
      /// \brief	   Constructor (initialize the static object)
      //--------------------------------------------------------------
      Provider(boost::shared_ptr<ICurrentTime> currentTimeInstance);

      //--------------------------------------------------------------
      /// \brief	   Destructor
      //--------------------------------------------------------------
      virtual ~Provider();

      //--------------------------------------------------------------
      /// \brief	   Returns now time
      /// \return Now
      //--------------------------------------------------------------
      static boost::posix_time::ptime now();

   private:
      //--------------------------------------------------------------
      /// \brief	   The currentTime object
      //--------------------------------------------------------------
      static boost::shared_ptr<ICurrentTime> CurrentTimeInstance;
   };

} } // namespace shared::currentTime
