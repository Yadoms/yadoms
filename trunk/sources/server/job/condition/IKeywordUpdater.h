#pragma once

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief Interface of notification when a keyword changes state
   //-----------------------------------------------------
   class IKeywordUpdater
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IKeywordUpdater() {}

      //-----------------------------------------------------
      ///\brief               Get the keyword ID
      //-----------------------------------------------------
      virtual int getKeywordId() const = 0;

      //-----------------------------------------------------
      ///\brief               Called when keyword state changes
      ///\param[in] state     new keyword state //TODO revoir le typage de l'�tat, ou au moins son traitement
      //-----------------------------------------------------
      virtual void onKeywordStateChange(const std::string& state) = 0;
   };
	
} } // namespace job::condition
	
	