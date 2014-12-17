#pragma once

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The root condition interface
   //-----------------------------------------------------
   class IConditionRoot
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IConditionRoot() {}

      //-----------------------------------------------------
      ///\brief               Evaluate the condition, unlock wait if condition becomes true
      //-----------------------------------------------------
      virtual void eval() = 0;

      //-----------------------------------------------------
      ///\brief               Wait for condition is met
      //-----------------------------------------------------
      virtual void wait() = 0;
   };
	
} } // namespace job::condition
	
	