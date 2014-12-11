#pragma once

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The keyword notifier interface
   //-----------------------------------------------------
   class IKeywordNotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IKeywordNotifier() {}

      //-----------------------------------------------------
      ///\brief               Get the keyword ID
      //-----------------------------------------------------
      virtual int getKeywordId() const = 0;

      //-----------------------------------------------------
      ///\brief               Called when keyword state changes
      //-----------------------------------------------------
      virtual void onNotify() const = 0;
   };
	
} } // namespace job::condition
	
	