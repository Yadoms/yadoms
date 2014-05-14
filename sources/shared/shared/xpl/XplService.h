#pragma once

#include <shared/Export.h>
#include "XplActor.h"
#include "XplMessage.h"
#include <shared/event/EventHandler.hpp>
#include <shared/StringExtension.h>

namespace shared { namespace xpl
{




   class YADOMS_SHARED_EXPORT CXplService
   {
   private:
      CXplService(CXplService&){}// Non-copyable

   public:

      //--------------------------------------------------------------
      /// \brief			Ctor using default network interface
      /// \param [in]   deviceId : The device Id delivered by Xpl Project
      /// \param [in]   instanceId : The instance Id
      /// \param [in]   externalIOService : a pointer to an external ioservice. if NULL a new ioservice is created.
      /// \note         Using this constructor implies that vendor Id will be CXplConstants::getYadomsVendorId
      //--------------------------------------------------------------
      CXplService(const std::string & deviceId, const std::string & instanceId,
         boost::asio::io_service * externalIOService = NULL);

      //--------------------------------------------------------------
      /// \brief			Ctor
      /// \param [in]   vendorId : The vendor Id delivered by Xpl Project
      /// \param [in]   deviceId : The device Id delivered by Xpl Project
      /// \param [in]   instanceId : The instance Id
      /// \param [in] : localIPOfTheInterfaceToUse : Local @ ip used to select network interface to send (if empty, use the first system IP found)
      /// \param [in]   externalIOService : a pointer to an external ioservice. if NULL a new ioservice is created
      //--------------------------------------------------------------
      CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId,
         const std::string & localIPOfTheInterfaceToUse = CStringExtension::EmptyString, boost::asio::io_service * externalIOService = NULL);   

      //--------------------------------------------------------------
      /// \brief			Dtor
      //--------------------------------------------------------------
      virtual ~CXplService();

      //--------------------------------------------------------------
      /// \brief			Stop the service
      //--------------------------------------------------------------
      void stop();

      //--------------------------------------------------------------
      /// \brief			Send the message to the remoteEndpoint
      /// \param [in]   message : The message to send
      //--------------------------------------------------------------
      void sendMessage(const CXplMessage & message);


      //--------------------------------------------------------------
      /// \brief		 Get the actor defined in the service
      /// \return		The current actor
      //--------------------------------------------------------------
      CXplActor getActor() const;




      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMessages(event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief		   Subscribe to receive all messages which are targetted to this XplService instance 
      /// \param [in]   pEventHandler                 The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier           The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForAllMyMessages(event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification = true);

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
      void subscribeForMessages(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId, event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification = true);

      //--------------------------------------------------------------
      /// \brief			Subscribe to receive all messages which match the filter defined by parameters
      /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
      /// \param [in]   pEventHandler        The event handler to notify when a message is received
      /// \param [in]   eventTypeIdentifier  The event id to use in notification
      /// \param [in]   continueChainingNotification  indicates if the notification queue should continue when a message match this filter
      //--------------------------------------------------------------
      void subscribeForMessages(const std::string & filter, event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification = true);


      //--------------------------------------------------------------
      /// \brief			Clear all subscription
      //--------------------------------------------------------------
      void clearAllSubscriptions();






   private:
      void initializeConnector();
      void heartbeatSequence(const boost::system::error_code& error);
      void runHeartbeatSequenceIn(const int seconds);
      void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

      void startService();


      boost::shared_ptr< boost::asio::deadline_timer > m_timer;

      boost::asio::ip::udp::endpoint m_localEndPoint;
      boost::asio::ip::udp::endpoint m_remoteEndPoint;

      boost::shared_ptr< boost::asio::ip::udp::socket > m_socket;

      boost::shared_ptr<boost::thread> m_serviceThread;

      bool m_hubHasBeenFound;
      boost::posix_time::ptime m_startDate;
      CXplActor m_source;

      boost::array<char, 1024> m_receiveBuffer;


      static const int HeartbeatInterval = 5;

      static const int HeartbeatFrequencyDuringInitialDiscoveryPhase = 5;
      static const int HeartbeatFrequencyDuringSecondDiscoveryPhase = 30;
      static const int HeartbeatFrequencyDuringNormalRun = HeartbeatInterval * 60;

      static const int HubDiscoveryTimeOut = 2 * 60;


      //--------------------------------------------------------------
      /// \brief			boost asio io_serice
      //--------------------------------------------------------------
      boost::asio::io_service * m_ioService;

      //--------------------------------------------------------------
      /// \brief			true if the ioService is manage by this instance, or false if it is given from owner
      //--------------------------------------------------------------
      bool m_manageIoService;


      //--------------------------------------------------------------
      /// \brief Class container for a message filter
      //--------------------------------------------------------------
      class CXplMessageFilter
      {
      public:
         //--------------------------------------------------------------
         /// \brief			Construct a filter to apply to the received messages
         /// \param [in]   msgtype     Type of message to receive (* allowed)
         /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
         /// \param [in]   device      Device from which to receive messages (* allowed)
         /// \param [in]   instance    Instance from which to receive messages (* allowed)
         /// \param [in]   classId     Class Id from which to receive messages (* allowed)
         /// \param [in]   typeId      Type of message to receive  (* allowed)
         //--------------------------------------------------------------
         CXplMessageFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId);

         //--------------------------------------------------------------
         /// \brief			Construct a filter to apply to the received messages
         /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
         //--------------------------------------------------------------
         CXplMessageFilter(const std::string & filter);

         //--------------------------------------------------------------
         /// \brief			Destructor
         //--------------------------------------------------------------
         virtual ~CXplMessageFilter();

         //--------------------------------------------------------------
         /// \brief			Check if an XplMessage match this filter
         /// \param [in]   msg   The XplMessage to check
         /// \return       True if the message match this filter
         //--------------------------------------------------------------
         bool match(const CXplMessage & msg);

      private:

         //--------------------------------------------------------------
         /// \brief			Configure the filter to apply to the received messages
         /// \param [in]   msgtype     Type of message to receive (* allowed)
         /// \param [in]   vendor      Vendor from which to receive messages (* allowed)
         /// \param [in]   device      Device from which to receive messages (* allowed)
         /// \param [in]   instance    Instance from which to receive messages (* allowed)
         /// \param [in]   classId     Class Id from which to receive messages (* allowed)
         /// \param [in]   typeId      Type of message to receive  (* allowed)
         //--------------------------------------------------------------
         void setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId);

         std::string m_msgtype;
         std::string m_vendor;
         std::string m_device;
         std::string m_instance;
         std::string m_classId;
         std::string m_typeId;
      };


      //--------------------------------------------------------------
      /// \brief			The filtering sytem
      /// This is a vector of tuple
      ///  tuple definition : 1. shared_ptr to the Filter 
      ///                     2. the event handler which receive notifications
      ///                     3. the event id to use for notification
      ///                     4. if false it stops the notification queue; if true try other filters
      //--------------------------------------------------------------
      typedef boost::tuple< boost::shared_ptr< CXplMessageFilter >, event::CEventHandler *, int , bool > FilterConfiguration;
      std::vector< FilterConfiguration > m_filteringSystem;

      //--------------------------------------------------------------
      /// \brief			Send notification to all configured subscribers
      /// \param [in]   message : The message to notify
      //--------------------------------------------------------------
      void notifySubscribers(CXplMessage & msg);
   };

} } // namespace shared::xpl
