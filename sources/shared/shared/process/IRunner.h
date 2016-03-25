#pragma once

namespace shared { namespace process
{
   //-----------------------------------------------------
   ///\brief The process runner interface
   //-----------------------------------------------------
   class IRunner
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IRunner() {}

      //-----------------------------------------------------
      ///\brief               Request to stop process
      //-----------------------------------------------------
      virtual void requestStop() = 0;
   };
	
} } // namespace shared::process
	
	