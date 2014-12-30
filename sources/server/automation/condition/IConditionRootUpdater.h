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
      virtual void onKeywordStateChange() = 0;
   };
	
} } // namespace automation::condition
	
	