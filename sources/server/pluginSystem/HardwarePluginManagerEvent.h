//
// HardwarePluginManager.h
//
// Yadoms-plugin manager
//
#pragma once


//--------------------------------------------------------------
/// \brief	    Event in the plugin manager
//--------------------------------------------------------------
class CHardwarePluginManagerEvent
{
public:
   //--------------------------------------------------------------
   /// \brief	    Type of plugin manager event
   //--------------------------------------------------------------
   enum ESubEventType
   {
      kPluginInstanceAbnormalStopped = 0,      // Plugin instance abnormal stopped
   };
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in]    eventId     Type of plugin manager event
   /// \param[in]    instanceId  Instance id where stop occurred
   /// \param[in]    pluginInformation Informations on plugin (name, version...)
   /// \param[in]    stopping true is crash occurred when plugin stopped
   //--------------------------------------------------------------
   CHardwarePluginManagerEvent(ESubEventType eventId, int instanceId, const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation, bool stopping)
      :m_eventId(eventId), m_instanceId(instanceId), m_pluginInformation(pluginInformation), m_stopping(stopping)
   {
   }

   //--------------------------------------------------------------
   /// \brief	   Type of plugin manager event getter
   /// \return    The plugin manager event type
   //--------------------------------------------------------------
   ESubEventType getSubEventId() const
   {
      return m_eventId;
   }

   //--------------------------------------------------------------
   /// \brief	   Id of the plugin instance getter
   /// \return    The plugin instance ID
   //--------------------------------------------------------------
   int getInstanceId() const
   {
      return m_instanceId;
   }

   //--------------------------------------------------------------
   /// \brief	   Plugin information getter
   /// \return    Information on plugin (name, version...)
   //--------------------------------------------------------------
   boost::shared_ptr<const IHardwarePluginInformation> getPluginInformation() const
   {
      return m_pluginInformation;
   }

   //--------------------------------------------------------------
   /// \brief	   Stopping state getter
   /// \return    true if the plugin instance was stopping when crashed
   //--------------------------------------------------------------
   bool getStopping() const
   {
      return m_stopping;
   }

private:
   //--------------------------------------------------------------
   /// \brief	   Type of plugin manager event
   //--------------------------------------------------------------
   ESubEventType m_eventId;

   //--------------------------------------------------------------
   /// \brief	   Id of the plugin instance
   //--------------------------------------------------------------
   int m_instanceId;

   //--------------------------------------------------------------
   /// \brief	   Plugin information
   //--------------------------------------------------------------
   const boost::shared_ptr<const IHardwarePluginInformation> m_pluginInformation;

   //--------------------------------------------------------------
   /// \brief	   Instance stopping state, when event occurred
   //--------------------------------------------------------------
   bool m_stopping;
};
