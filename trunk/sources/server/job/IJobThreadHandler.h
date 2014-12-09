#pragma once

namespace job
{
   //-----------------------------------------------------
   ///\brief The job thread handler interface
   //-----------------------------------------------------
   class IJobThreadHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IJobThreadHandler() {}

      //-----------------------------------------------------
      ///\brief               The thread function
      //-----------------------------------------------------
      virtual void doWork() = 0;
   };
	
} // namespace job	
	
	