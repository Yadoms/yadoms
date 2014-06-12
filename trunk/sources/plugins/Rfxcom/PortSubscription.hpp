#pragma once

#include <shared/exception/InvalidParameter.hpp>


//--------------------------------------------------------------
/// \brief	This class manage a subscription for events on communication port
//--------------------------------------------------------------
template<class DataType>
class CPortSubscription
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPortSubscription()
      :m_eventHandler(NULL), m_id(shared::event::kNoEvent)
   {
   }

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPortSubscription()
   {
   }

   //--------------------------------------------------------------
   /// \brief	Set the subscription
   /// \param [in] forEventHandler  The event handler to notify for these events
   /// \param [in] forId            The event id to send for these events (set kNoEvent to unsubscribe)
   /// \throw shared::exception::CInvalidParameter if try to subscribe on event for which a subscription already exists (user must unsubscribe first)
   //--------------------------------------------------------------
   void subscribe(shared::event::CEventHandler& forEventHandler, int forId)
   {
      // Check for unsubscription request
      if (forId == shared::event::kNoEvent)
         unsubscribe();

      // Subscription

      // Check if subscription already exists
      if (hasSubscription())
         throw shared::exception::CInvalidParameter("SerialPort : subscription for connection state already exists");

      // Do the subscription
      boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
      m_eventHandler = &forEventHandler;
      m_id = forId;
   }

   //--------------------------------------------------------------
   /// \brief	Cancel the subscription
   //--------------------------------------------------------------
   void unsubscribe()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
      m_eventHandler = NULL;
   }

   //--------------------------------------------------------------
   /// \brief	Get the subscription state
   /// \return true if subscription exists, false else
   //--------------------------------------------------------------
   bool hasSubscription() const
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
      return m_eventHandler != NULL;
   }

   //--------------------------------------------------------------
   /// \brief	Notify the subscriber
   //--------------------------------------------------------------
   void notify()
   {
      if (!hasSubscription())
         return;

      boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
      m_eventHandler->postEvent(m_id);
   }

   //--------------------------------------------------------------
   /// \brief	Notify the subscriber, sending data
   //--------------------------------------------------------------
   void notify(const DataType& value)
   {
      if (!hasSubscription())
         return;

      boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
      m_eventHandler->postEvent<DataType>(m_id, value);
   }

private:
   //--------------------------------------------------------------
   /// \brief	Mutex protecting the subscription
   //--------------------------------------------------------------
   mutable boost::recursive_mutex m_mutex;

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for events
   //--------------------------------------------------------------
   shared::event::CEventHandler* m_eventHandler;

   //--------------------------------------------------------------
   /// \brief	The event id to send
   //--------------------------------------------------------------
   int m_id;
};



