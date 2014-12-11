#pragma once
#include <shared/shared/DataContainer.h>

namespace job { namespace action
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
      virtual void doAction() const = 0;
   };
	
} } // namespace job::action	
	
	