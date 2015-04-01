#pragma once
#include "INotifier.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace configurationUpdate
{
   //-----------------------------------------------------
   ///\brief The configuration update notifier
   //-----------------------------------------------------
   class CNotifier : public INotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CNotifier();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNotifier();

   protected:
      // INotifier implementation
      virtual void subscribe(boost::shared_ptr<IObserver> observer);
      virtual void unsubscribe(boost::shared_ptr<IObserver> observer);
      virtual void post(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration);
      // [END] INotifier implementation

   private:
      //-----------------------------------------------------
      ///\brief               Subscribed observers
      //-----------------------------------------------------
      std::set<boost::shared_ptr<IObserver> > m_observers;

      //-----------------------------------------------------
      ///\brief               Mutex for subscribed observers
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_observersMutex;
   };

} } // namespace notification::configurationUpdate
	
	