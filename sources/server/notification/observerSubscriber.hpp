#pragma once

namespace notification
{
   //-----------------------------------------------------
   ///\brief This object subscribe an observer for acquisition notifications
   //-----------------------------------------------------
   template<class NotifierInterface, class ObserverInterface>
   class CObserverSubscriber
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] notifier  The acquisition notifier
      ///\param[in] observer  The observer
      //-----------------------------------------------------
      CObserverSubscriber(boost::shared_ptr<NotifierInterface> notifier, boost::shared_ptr<ObserverInterface> observer)
         :m_notifier(notifier), m_observer(observer)
      {
         m_notifier->subscribe(m_observer);
      }

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserverSubscriber()
      {         
         m_notifier->unsubscribe(m_observer);
      }

   private:
      //-----------------------------------------------------
      ///\brief               Notification center
      //-----------------------------------------------------
      boost::shared_ptr<NotifierInterface> m_notifier;

      //-----------------------------------------------------
      ///\brief               Observer
      //-----------------------------------------------------
      boost::shared_ptr<ObserverInterface> m_observer;
   };

} // namespace notification
	
	