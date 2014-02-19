#pragma once

#include <shared/Export.h>
#include "XplActor.h"
#include "XplMessage.h"
#include <shared/Event/EventHandler.hpp>



class YADOMS_SHARED_EXPORT CXplService 
{
public:
   typedef boost::signals2::signal<void (CXplMessage&)> SigMessageReceived;
   typedef SigMessageReceived::slot_type SigMessageReceivedDelegate;
   
   //--------------------------------------------------------------
   /// \brief			Ctor using default network interface
   /// \param [in]   vendorId : The vendor Id delivered by Xpl Project
   /// \param [in]   deviceId : The device Id delivered by Xpl Project
   /// \param [in]   instanceId : The instance Id
   //--------------------------------------------------------------
   CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId, boost::shared_ptr< boost::asio::io_service > pluginIOService = NULL);
   
   //--------------------------------------------------------------
   /// \brief			Ctor
   /// \param [in]   vendorId : The vendor Id delivered by Xpl Project
   /// \param [in]   deviceId : The device Id delivered by Xpl Project
   /// \param [in]   instanceId : The instance Id
   /// \param [in] : localIPOfTheInterfaceToUse : Local @ ip used to select network interface to send
   //--------------------------------------------------------------
   CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId, const std::string & localIPOfTheInterfaceToUse, boost::shared_ptr< boost::asio::io_service > pluginIOService = NULL);   
   
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
   /// \brief			Record the given method to the event Message received
   /// \param [in]   dlg : the method to call when a message is received (ex : boost::bind(&CClass::onMessageReceived, this, _1)
   /// \example      xplService->messageReceived(boost::bind(&CMyClass::onMessageReceived, this, _1));
   ///               void CFakePlugin::onMessageReceived(CXplMessage & message) { treat message here }
   //--------------------------------------------------------------
   void messageReceived(const SigMessageReceivedDelegate &dlg);

   //--------------------------------------------------------------
   /// \brief			Record an event handler to notify it when a message is receeived
   /// \param [in]   handler : a shared_ptr on the eventhandler
   /// \param [in]   eventTypeIdentifier : the event type to generate when an XplMessage is received
   /// \eexample     xplService->messageReceived(shared_from_this(), kEvtXplMessage);
   //--------------------------------------------------------------
   void messageReceived(boost::shared_ptr< CEventHandler > handler, const int eventTypeIdentifier);

   //--------------------------------------------------------------
   /// \brief			Remove all methods which handles the event Message received
   //--------------------------------------------------------------
   void removeAllHandlers();
    
   //--------------------------------------------------------------
   /// \brief			Configure the filter to apply to the received messages
   /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
   //--------------------------------------------------------------
   void setFilter(const std::string & filter);

   //--------------------------------------------------------------
   /// \brief			Configure the filter to apply to the received messages
   /// \param [in]   filter to apply. It must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (* is allowed for each param)
   //--------------------------------------------------------------
   void setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, 
                  const std::string & classId, const std::string & typeId);

   //--------------------------------------------------------------
   /// \brief		 Get the actor defined in the service
   /// \return		The current actor
   //--------------------------------------------------------------
   CXplActor getActor() const;

private:

   struct SFilter
   {
      boost::shared_ptr<std::string> msgtype;
      boost::shared_ptr<std::string> vendor;
      boost::shared_ptr<std::string> device;
      boost::shared_ptr<std::string> instance;
      boost::shared_ptr<std::string> classId;
      boost::shared_ptr<std::string> typeId;
   };

   void initializeConnector();
   void heartbeatSequence();
   void runHeartbeatSequenceIn(const int seconds);
   void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

   void startService();
   //--------------------------------------------------------------
   /// \brief			Fire the event MessageReceived
   /// \param [in]   message : The message which just has been received
   //--------------------------------------------------------------
   void fireMessageReceivedEvent(CXplMessage & msg);

   //--------------------------------------------------------------
   /// \brief			Utility method which call m_sigMessageReceived
   ///               Only called in a thread to ensure non blocking calls
   /// \param [in]   message : The message which just has been received
   //--------------------------------------------------------------
   void asyncSendMessage(CXplMessage & msg);

   boost::shared_ptr< boost::asio::deadline_timer > m_timer;

   boost::asio::ip::udp::endpoint m_localEndPoint;
   boost::asio::ip::udp::endpoint m_remoteEndPoint;
   
   boost::shared_ptr< boost::asio::ip::udp::socket > m_socket;

   boost::shared_ptr<boost::thread> m_serviceThread;

   bool m_hubHasBeenFound;
   boost::posix_time::ptime m_startDate;
   CXplActor m_source;

   boost::array<char, 1024> m_receiveBuffer;

   SigMessageReceived m_sigMessageReceived;
   
   boost::shared_ptr< CEventHandler > m_eventHandler;
   int m_eventIdToSignal;

   SFilter m_filter;

   static const int HeartbeatInterval = 1;//5;

   static const int HeartbeatFrequencyDuringInitialDiscoveryPhase = 5;
   static const int HeartbeatFrequencyDuringSecondDiscoveryPhase = 30;
   static const int HeartbeatFrequencyDuringNormalRun = HeartbeatInterval * 60;

   static const int HubDiscoveryTimeOut = 2 * 60;


   //--------------------------------------------------------------
   /// \brief			boost asio io_serice
   //--------------------------------------------------------------
   boost::shared_ptr< boost::asio::io_service > m_ioService;

   //--------------------------------------------------------------
   /// \brief			true if the ioService is manage by this instance, or false if it is given from owner
   //--------------------------------------------------------------
   bool m_manageIoService;
};

