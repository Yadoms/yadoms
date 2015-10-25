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
         kRuleStopped
      };

   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in]    eventId     Type of plugin manager event
      /// \param[in]    ruleId      Rule id where stop occurred
      /// \param[in]    error       Error associated to event (empty if not error)
      //--------------------------------------------------------------
      CManagerEvent(ESubEventType eventId, int ruleId, const std::string& error = std::string())
         :m_eventId(eventId), m_ruleId(ruleId), m_error(error)
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

      //--------------------------------------------------------------
      /// \brief	   Error associated to event
      /// \return    The error (empty if no error)
      //--------------------------------------------------------------
      std::string getError() const
      {
         return m_error;
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

      //--------------------------------------------------------------
      /// \brief	   Error label
      //--------------------------------------------------------------
      const std::string m_error;
   };

} // namespace automation
