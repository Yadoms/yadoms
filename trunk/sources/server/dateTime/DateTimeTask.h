#pragma once
#include <Poco/Util/Timer.h>

namespace dateTime {

   class CDateTimeTask : public Poco::Util::TimerTask
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CDateTimeTask();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDateTimeTask();

      // BEGIN Poco::Util::TimerTask implementation 
      virtual void run();
      // END Poco::Util::TimerTask implementation 

   };

} //namespace dateTime