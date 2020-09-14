#pragma once

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    An simple event (without data)
      //--------------------------------------------------------------
      class CEventBase
      {
      public:
         //--------------------------------------------------------------
         /// \brief	    Constructor
         /// \param[in] id Event id
         //--------------------------------------------------------------
         explicit CEventBase(int id)
            : m_id(id)
         {
         }

         //--------------------------------------------------------------
         /// \brief	    Destructor
         //--------------------------------------------------------------
         virtual ~CEventBase() = default;

         //--------------------------------------------------------------
         /// \brief	    Copy
         //--------------------------------------------------------------
         CEventBase(const CEventBase&) = default;
         CEventBase& operator=(const CEventBase&) = default;

         //--------------------------------------------------------------
         /// \brief	    Move
         //--------------------------------------------------------------
         CEventBase(const CEventBase&&) = delete;
         CEventBase& operator=(const CEventBase&&) = delete;

         //--------------------------------------------------------------
         /// \brief	    Id getter
         /// \return     Event id
         //--------------------------------------------------------------
         int getId() const { return m_id; }

      private:
         //--------------------------------------------------------------
         /// \brief	    Event id
         //--------------------------------------------------------------
         int m_id;
      };
   }
} // namespace shared::event
