#pragma once

#include "XplActor.h"
#include "XplMessage.h"
#include "XplMessageFilter.h"
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Task.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

namespace xplcore
{
   //--------------------------------------------------------------
   /// \brief		  This class implements a Xpl service on a network interface
   //--------------------------------------------------------------
   class CXplServiceTask : public Poco::Task
   {
   public:
      //--------------------------------------------------------------
      /// \brief		  Constructor
      /// \param [in]   networkInterface        The network interface to use
      /// \param [in]   vendor                  Vendor id (for xpl network identification)
      /// \param [in]   device                  Device  (for xpl network identification)
      /// \param [in]   instance                Instance  (for xpl network identification)
      /// \param [in]   pHubFoundEventHandler   The event handler to notify when a hub is found
      /// \param [in]   hubFoundEventCode       The event id to use in "hub found" notification
      //--------------------------------------------------------------
      CXplServiceTask(Poco::Net::NetworkInterface& networkInterface,
                      const std::string& vendorId,
                      const std::string& deviceId,
                      const std::string& instanceId,
                      shared::event::CEventHandler* pHubFoundEventHandler,
                      int hubFoundEventCode);

      //--------------------------------------------------------------
      /// \brief		  Destructor
      //--------------------------------------------------------------
      virtual ~CXplServiceTask();

      //--------------------------------------------------------------
      /// \brief		  The main working method (listen the xpl socket)
      //--------------------------------------------------------------
      void runTask() override;

      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMessages(shared::event::CEventHandler* pEventHandler,
                                   int eventTypeIdentifier,
                                   bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages which are targeted to this XplService instance 
      /// \param [in]   pEventHandler                 The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier           The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMyMessages(shared::event::CEventHandler* pEventHandler,
                                     int eventTypeIdentifier,
                                     bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief			Subscribe to receive all messages which match the filter defined by parameters
      /// \param [in]   msgtype     Type of message to receive (* allowed)
      /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
      /// \param [in]   device      Device from which to receive messages (* allowed)
      /// \param [in]   instance    Instance from which to receive messages (* allowed)
      /// \param [in]   classId     Class Id from which to receive messages (* allowed)
      /// \param [in]   typeId      Type of message to receive  (* allowed)
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForMessages(const std::string& msgtype,
                                const std::string& vendor,
                                const std::string& device,
                                const std::string& instance,
                                const std::string& classId,
                                const std::string& typeId,
                                shared::event::CEventHandler* pEventHandler,
                                int eventTypeIdentifier,
                                bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief			Subscribe to receive all messages which match the filter defined by parameters
      /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForMessages(const std::string& filter,
                                shared::event::CEventHandler* pEventHandler,
                                int eventTypeIdentifier,
                                bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief			Clear all subscription
      //--------------------------------------------------------------
      void clearAllSubscriptions();

      //--------------------------------------------------------------
      /// \brief			Send a message
      /// \param [in]   msg      The message
      //--------------------------------------------------------------
      void sendMessage(const CXplMessage& msg);

      //--------------------------------------------------------------
      /// \brief		 Get the actor defined in the service
      /// \return		The current actor
      //--------------------------------------------------------------
      CXplActor getActor() const;

   private:
      //--------------------------------------------------------------
      /// \brief			Initialize the service
      /// \param [in]   networkInterface  The network interface to use
      //--------------------------------------------------------------
      void initializeConnector(Poco::Net::NetworkInterface& networkInterface);

      //--------------------------------------------------------------
      /// \brief			Start a heartbeat sequence
      /// \param [in]   seconds  Amount of second to wait before starting the sequence
      //--------------------------------------------------------------
      void runHeartbeatSequenceIn(const int seconds);

      //--------------------------------------------------------------
      /// \brief			heartbeat sequence
      /// \param [in]   timer  The timer
      //--------------------------------------------------------------
      void heartbeatSequence();

      //--------------------------------------------------------------
      /// \brief			Treat an xpl message received
      /// \param [in]   msg      The message
      /// \param [in]   sender   The sender
      //--------------------------------------------------------------
      void onXplMessageReceived(CXplMessage& msg,
                                Poco::Net::SocketAddress& sender);

      //--------------------------------------------------------------
      /// \brief			Treat a received heartbeat message
      /// \param [in]   msg      The message
      /// \param [in]   sender   The sender
      //--------------------------------------------------------------
      void manageReceivedHeartBeatMessage(const CXplMessage& hbeatMessage,
                                          Poco::Net::SocketAddress& sender);

      //--------------------------------------------------------------
      /// \brief			Send notification to all configured subscribers
      /// \param [in]   message : The message to notify
      //--------------------------------------------------------------
      void notifySubscribers(CXplMessage& msg);


      //info
      boost::posix_time::ptime m_startDate;

      //Hub management
      bool m_hubHasBeenFound;
      shared::event::CEventHandler* m_pHubFoundEventHandler;
      int m_hubFoundEventCode;
      static const int HubDiscoveryTimeOut = 2 * 60;

      //Heartbeat intervals
      static const int HeartbeatInterval = 5;
      static const int HeartbeatFrequencyDuringInitialDiscoveryPhase = 5;
      static const int HeartbeatFrequencyDuringSecondDiscoveryPhase = 30;
      static const int HeartbeatFrequencyDuringNormalRun = HeartbeatInterval * 60;

      //poco objects
      Poco::Net::DatagramSocket m_socket;
      Poco::Net::SocketAddress m_localEndPoint;
      Poco::Net::SocketAddress m_remoteEndPoint;
      boost::posix_time::ptime m_nextHeartbeatTime;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the socket access
      //--------------------------------------------------------------
      mutable boost::mutex m_socketLocker;

      //--------------------------------------------------------------
      /// \brief			The filtering sytem
      /// This is a vector of tuple
      ///  tuple definition : 1. shared_ptr to the Filter 
      ///                     2. the event handler which receive notifications
      ///                     3. the event id to use for notification
      ///                     4. if false it stops the notification queue; if true try other filters
      //--------------------------------------------------------------
      typedef boost::tuple<boost::shared_ptr<CXplMessageFilter>, shared::event::CEventHandler *, int, bool> FilterConfiguration;
      std::vector<FilterConfiguration> m_filteringSystem;

      //xpl data
      CXplActor m_source;
   };
} // namespace xplcore


