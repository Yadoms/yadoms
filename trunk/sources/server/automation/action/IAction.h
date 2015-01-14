#pragma once
#include <shared/shared/DataContainer.h>

namespace automation { namespace action
{
   //-----------------------------------------------------
   ///\brief An action
   //-----------------------------------------------------
   class IAction
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IAction() {}

      //-----------------------------------------------------
      ///\brief               Do the action
      //-----------------------------------------------------
      virtual void doAction() = 0;

      //-----------------------------------------------------
      ///\brief               Stop the action
      //-----------------------------------------------------
      virtual void stopAction() = 0;
   };
	
} } // namespace automation::action	
	
	