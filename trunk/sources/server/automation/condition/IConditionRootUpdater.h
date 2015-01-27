#pragma once

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The root condition interface
   //-----------------------------------------------------
   class IConditionRootUpdater
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IConditionRootUpdater() {}

      //-----------------------------------------------------
      ///\brief               Called when keyword state changes
      //-----------------------------------------------------
      virtual void requestEvaluation() = 0;
   };
	
} } // namespace automation::condition
	
	