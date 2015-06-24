#pragma once

namespace automation
{
   //--------------------------------------------------------------
   /// \brief	    Event in the autoamtion rules manager
   //--------------------------------------------------------------
   class CManagerEvent
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Type of rule manager event
      //--------------------------------------------------------------
      enum ESubEventType
      {
         kRuleAbnormalStopped = 0,      // Rule abnormal stopped
      };

   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in]    eventId     Type of plugin manager event
      /// \param[in]    ruleId      Rule id where stop occurred
      //--------------------------------------------------------------
      CManagerEvent(ESubEventType eventId, int ruleId)
         :m_eventId(eventId), m_ruleId(ruleId)
      {
      }

      //--------------------------------------------------------------
      /// \brief	   Destructor
      //--------------------------------------------------------------
      virtual ~CManagerEvent() {}

      //--------------------------------------------------------------
      /// \brief	   Type of event getter
      /// \return    The event type
      //--------------------------------------------------------------
      ESubEventType getSubEventId() const
      {
         return m_eventId;
      }

      //--------------------------------------------------------------
      /// \brief	   Id of the rule
      /// \return    The rule ID
      //--------------------------------------------------------------
      int getRuleId() const
      {
         return m_ruleId;
      }

   private:
      //--------------------------------------------------------------
      /// \brief	   Type of plugin manager event
      //--------------------------------------------------------------
      ESubEventType m_eventId;

      //--------------------------------------------------------------
      /// \brief	   Id of the rule
      //--------------------------------------------------------------
      int m_ruleId;
   };

} // namespace automation
