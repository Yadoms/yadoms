#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief The rule thread handler interface
   //-----------------------------------------------------
   class IRuleThreadHandler
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRuleThreadHandler() {}

      //-----------------------------------------------------
      ///\brief               The thread function
      //-----------------------------------------------------
      virtual void doWork() = 0;
   };
	
} // namespace automation	
	
	