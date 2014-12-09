#pragma once

namespace job
{
   //-----------------------------------------------------
   ///\brief The condition interface
   //-----------------------------------------------------
   class ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ICondition() {}

      //-----------------------------------------------------
      ///\brief               Wait for condition is met
      //-----------------------------------------------------
      virtual void wait() const = 0;
   };
	
} // namespace job	
	
	