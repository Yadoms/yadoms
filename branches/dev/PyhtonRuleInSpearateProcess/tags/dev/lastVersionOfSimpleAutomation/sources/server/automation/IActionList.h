#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief The list of actions interface
   //-----------------------------------------------------
   class IActionList
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IActionList() {}

      //-----------------------------------------------------
      ///\brief               Do all actions
      //-----------------------------------------------------
      virtual void doAll() = 0;

      //-----------------------------------------------------
      ///\brief               Stop pendings action
      ///\note implementations of this function must not block
      //-----------------------------------------------------
      virtual void stopPending() = 0;
   };
	
} // namespace automation	
	
	