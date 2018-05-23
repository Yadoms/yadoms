#pragma once
#include "EventBase.hpp"

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    An event containing data
      /// \template T  Type of the data in the event (must be copyable)
      //--------------------------------------------------------------
      template <typename T>
      class CEvent : public CEventBase
      {
      public:
         //--------------------------------------------------------------
         /// \brief	    Constructor
         /// \param[in] id Event id
         /// \param[in] data Event data (must be copyable)
         /// \note      data will be copied locally so it can be disallowed after this call
         //--------------------------------------------------------------
         CEvent(int id, const T& data)
            : CEventBase(id), m_data(data)
         {
         }

         //--------------------------------------------------------------
         /// \brief	    Destructor
         //--------------------------------------------------------------
         virtual ~CEvent()
         {
         }

         //--------------------------------------------------------------
         /// \brief	    Data getter
         /// \return     Event data
         //--------------------------------------------------------------
         const T& getData() const { return m_data; }

      private:
         //--------------------------------------------------------------
         /// \brief	    Local copy of event data
         //--------------------------------------------------------------
         const T m_data;
      };
   }
} // namespace shared::event
