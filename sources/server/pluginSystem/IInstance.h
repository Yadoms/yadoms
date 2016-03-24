#pragma once

namespace automation
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class IInstance
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstance() {}

      //-----------------------------------------------------
      ///\brief               Request to stop instance
      //-----------------------------------------------------
      virtual void requestStop() = 0;
   };
	
} // namespace automation	
	
	