#include "stdafx.h"
#include "XplService.h"

#include "../Log.h"
#include "XplException.h"
#include "XplMessage.h"
#include "XplMessageFactory.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplConstants.h"
#include "../StringExtension.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// A client send its data as broadcast on the XPL port,
// and listen on a certain port, given in its XPL frame.

// A hub is listening the XPL port, repeat all received data to the connected clients on their respective ports.

namespace shared { namespace xpl
{

   CXplService::CXplService(
      const std::string & deviceId,
      const std::string & instanceId,
      boost::asio::io_service * externalIOService)
      : m_manageIoService(externalIOService == NULL)
   {
      if(m_manageIoService)
         m_ioService = new boost::asio::io_service();
      else
         m_ioService = externalIOService;

      m_timer.reset( new boost::asio::deadline_timer(*m_ioService) );
      m_socket.reset( new boost::asio::ip::udp::socket(*m_ioService) );

      m_localEndPoint = CXplHelper::getFirstIPV4AddressEndPoint();
      m_source = CXplActor::createActor(CXplConstants::getYadomsVendorId(), deviceId, instanceId);

      initializeConnector();
   }

   CXplService::CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId, const std::string & localIPOfTheInterfaceToUse, boost::asio::io_service * externalIOService)
      : m_manageIoService(externalIOService == NULL)
   {
      if(m_manageIoService)
         m_ioService = new boost::asio::io_service();
      else
         m_ioService = externalIOService;

      m_timer.reset( new boost::asio::deadline_timer(*m_ioService) );
      m_socket.reset( new boost::asio::ip::udp::socket(*m_ioService) );

      if (localIPOfTheInterfaceToUse.empty() || !CXplHelper::getEndPointFromInterfaceIp(localIPOfTheInterfaceToUse, m_localEndPoint))
      {
         //If we haven't found the given ip, we take the first address IPV4
         m_localEndPoint = CXplHelper::getFirstIPV4AddressEndPoint();
      }

      m_source = CXplActor::createActor(vendorId, deviceId, instanceId);
      initializeConnector();
   }

   CXplService::~CXplService()
   {
      stop();
      if(m_manageIoService)
         delete m_ioService;
   }

   void CXplService::initializeConnector()
   {
      m_hubHasBeenFound = false;
      m_startDate = boost::posix_time::second_clock::universal_time();

      runHeartbeatSequenceIn(HeartbeatFrequencyDuringInitialDiscoveryPhase);

      //we configure the socket
      m_socket->open(boost::asio::ip::udp::v4());
      m_socket->set_option(boost::asio::ip::udp::socket::reuse_address(true));
      m_socket->set_option(boost::asio::socket_base::broadcast(true));
      m_socket->bind(m_localEndPoint);

      m_remoteEndPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), CXplHelper::XplProtocolPort);

      YADOMS_LOG(debug) << "CXplService : Local EndPoint: " << m_localEndPoint.address().to_string() << " on port : " << m_localEndPoint.port();
      YADOMS_LOG(debug) << "CXplService : Remote EndPoint: " << m_remoteEndPoint.address().to_string() << " on port : " << m_remoteEndPoint.port();

      if(m_manageIoService)
      {
         m_serviceThread.reset(new boost::thread(boost::bind(&CXplService::startService, this)));
         YADOMS_LOG(debug) << "Thread Id=" << m_serviceThread->get_id() << " Name = CXplService IOService (managed by xplservice)";
      }

      m_socket->async_receive(
         boost::asio::buffer(m_receiveBuffer),
         boost::bind(&CXplService::handleReceive, this,
         boost::asio::placeholders::error,
         boost::asio::placeholders::bytes_transferred));


   }

   void CXplService::startService()
   {
      try
      {
         YADOMS_LOG_CONFIGURE(m_source.toString());
         if(m_ioService !=NULL)
            m_ioService->run();
      }
      catch (std::exception& e)
      {
         // Deal with exception as appropriate.
         YADOMS_LOG(warning) << "CXplService io_service exception : " << m_source.toString() << std::endl << "Exeption details : " << e.what();
      }
   }

   void CXplService::runHeartbeatSequenceIn(const int seconds)
   {
      if(m_ioService !=NULL && !m_ioService->stopped())
      {
         //calling expires_from_now cancel the timer, so, from a previous call to async_wait, the heartbeatSequence is called
         //if a previous call of async_wait has been done, just ignore the call to the method with error_code checking
         m_timer->expires_from_now(boost::posix_time::seconds(seconds));
         m_timer->async_wait(boost::bind(&CXplService::heartbeatSequence, this, _1));
      }
   }

   void CXplService::heartbeatSequence(const boost::system::error_code& error)
   {
      //check IOService is running
      //check error code from parameter is not the operation_aborted.
      //because when running runHeartbeatSequenceIn, the expire_from_now cancels the timer so it run into this function
      //then the async_wait is called
      if(m_ioService !=NULL && !m_ioService->stopped() && error != boost::asio::error::operation_aborted)
      {
         YADOMS_LOG(debug) << "hbeat";

         int heartbeatInterval;
         //depending on the mode we currently are we select the time to send another hbeat
         if (m_hubHasBeenFound)
         {
            //the hub have been found so we send a hbeat every 5 minutes to 30 minutes
            heartbeatInterval = HeartbeatFrequencyDuringNormalRun;
         }
         else
         {
            //the hub havn't been found for the moment
            boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
            boost::posix_time::time_duration diff = now - m_startDate;
            if (diff.total_seconds() > HubDiscoveryTimeOut)
            {
               //the hub haven't been found in 2 minutes so we send a hbeat every 30 seconds
               heartbeatInterval = HeartbeatFrequencyDuringSecondDiscoveryPhase;
            }
            else
            {
               //the hub havn't been found for the moment
               heartbeatInterval = HeartbeatFrequencyDuringInitialDiscoveryPhase;
            }
         }


         if(m_socket->is_open())
         {
            //We send the hbeat
            //we set always the interval of HeartbeatInterval in the message
            CXplMessage msg = CXplMessageFactory::createHeartbeatAppMessage(m_source, HeartbeatInterval, m_socket->local_endpoint().address().to_string(), m_socket->local_endpoint().port());
            try
            {
               m_socket->send_to(boost::asio::buffer(msg.toString()), m_remoteEndPoint);
            }
            catch(...)
            {
            }

            //We relaunch the hbeat
            runHeartbeatSequenceIn(heartbeatInterval);
         }
      }
   }

   void CXplService::handleReceive(const boost::system::error_code& error,
      std::size_t bytes_transferred)
   {
      if(m_ioService !=NULL && !m_ioService->stopped())
      {
         if (!error || error == boost::asio::error::message_size)
         {
            try
            {
               //we copy buffer to a string
               std::string data;
               std::copy(m_receiveBuffer.begin(), m_receiveBuffer.begin()+bytes_transferred, std::back_inserter(data));

               CXplMessage msg = CXplMessage::parse(data);

               //the message is successfully parsed
               YADOMS_LOG(trace) << "Message received : " << msg.toString();
               //When the hub receives a hbeat.app or config.app message, the hub should extract the "remote-ip" value 
               //from the message body and compare the IP address with the list of addresses the hub is currently bound 
               //to for the local computer. If the address does not match any local addresses, 
               //the packet moves on to the delivery/rebroadcast step.
               if(CXplMessageSchemaIdentifier::isHeartbeatMessageSchemaIdentifier(msg.getMessageSchemaIdentifier()))
               {
                  if(CXplMessageSchemaIdentifier::isHeartbeatApp(msg.getMessageSchemaIdentifier()))
                  {
                     //we've got an heartbeat if it's our hbeat and we are m_hubHasBeenFound = false we have found a hub
                     int port;
                     if (CStringExtension::tryParse<int>(msg.getBodyValue("port"), port))
                     {
                        if ((!m_hubHasBeenFound) && (m_localEndPoint.address().to_string() == msg.getBodyValue("remote-ip")) && (m_socket->local_endpoint().port() == port))
                        {
                           YADOMS_LOG(info) << "Hub found";
                           m_hubHasBeenFound = true;
                        }
                        else
                        {
                           //From another ip, nothing to do
                        }
                     }
                     else
                     {
                        //invalid heatbeat app message
                     }
                  }
                  else if(CXplMessageSchemaIdentifier::isHeartbeatRequest(msg.getMessageSchemaIdentifier()))
                  {
                     //it is a hbeat request from another client to get our information
                     //for the moment we don't manage it (it is not mandatory)
                     boost::random::mt19937 gen;
                     boost::random::uniform_int_distribution<> dist(2, 6);
                     runHeartbeatSequenceIn(dist(gen));
                  }
                  else
                  {
                     //another heartbeat which is neither app nor heartbeat
                  }
               }
               else
               {
                  //it is a normal message, forward notification to subscribers
                  notifySubscribers(msg);
               }
            }
            catch (std::exception & e)
            {
               std::cerr << e.what() << std::endl;
            }      
         }

         if(m_socket->is_open())
         {
            m_socket->async_receive(
               boost::asio::buffer(m_receiveBuffer),
               boost::bind(&CXplService::handleReceive, this,
               boost::asio::placeholders::error,
               boost::asio::placeholders::bytes_transferred));
         }
      }
      else
      {
         //asio service is stopped, do nothing
         YADOMS_LOG(info) << "CXplService::handleReceive : asio io_service is stopped";
      }
   }

   void CXplService::sendMessage(const CXplMessage & message)
   {
      m_socket->send_to(boost::asio::buffer(message.toString()), m_remoteEndPoint);
   }

   void CXplService::notifySubscribers(CXplMessage & msg)
   {
      bool atLeastOneNotificationSend = false;

      BOOST_FOREACH(FilterConfiguration subscriber, m_filteringSystem)
      {
         boost::shared_ptr< CXplMessageFilter > filter = subscriber.get<0>();
         if(filter && filter->match(msg))
         {
            event::CEventHandler * pEventHandler = subscriber.get<1>();
            if(pEventHandler)
            {
               //send notification
               pEventHandler->postEvent<CXplMessage>(subscriber.get<2>(), boost::ref(msg));

               //
               atLeastOneNotificationSend = true;

               //check for notification chaining
               bool continueNotificationChaining = subscriber.get<3>();

               //if do not continue chaing, just break the foreach
               if(!continueNotificationChaining)
                  break;
            }
         }
      }

      if(!atLeastOneNotificationSend)
      {
         YADOMS_LOG(debug) << "Message filtered";
      }
   }



   void CXplService::stop()
   {
      //stop io_service, must be done first
      if(m_manageIoService)
      {
         if(m_ioService != NULL && !m_ioService->stopped())
         {
            m_ioService->stop();
            while(!m_ioService->stopped());
            m_ioService = NULL;
         }

         if(m_serviceThread.get())
            m_serviceThread->join();
      }

      //cancel the timer
      m_timer->cancel();

      //stop the socket (shutdown, then close)
      try
      {
         m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      }
      catch(std::exception &ex)
      {
         YADOMS_LOG(warning) << "Fail to shutdown socket : " << ex.what();
      }
      m_socket->close();


      YADOMS_LOG(debug) << "CXplService stopped.";
   }

   CXplActor CXplService::getActor() const
   {
      return m_source;
   }




   CXplService::CXplMessageFilter::CXplMessageFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId)
   {
      setFilter(msgtype, vendor, device, instance, classId, typeId);
   }

   CXplService::CXplMessageFilter::CXplMessageFilter(const std::string & filter)
   {
      std::string lineString = boost::trim_copy(filter);

      //we allow a single "*" for all filter instead of *.*.*.*.*.*
      if (lineString != CXplHelper::WildcardString)
      {
         std::vector<std::string> line;
         boost::split(line, lineString, boost::is_any_of("."), boost::token_compress_on);
         if (line.size() != 6)
            throw CXplException("The filter string must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (wildchar * is allowed). Value given : " + filter);

         setFilter(line[0], line[1], line[2], line[3], line[4], line[5]);
      }
   }

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   CXplService::CXplMessageFilter::~CXplMessageFilter()
   {
   }


   //--------------------------------------------------------------
   /// \brief			Check if an XplMessage match this filter
   /// \param [in]   msg   The XplMessage to check
   /// \return       True if the message match this filter
   //--------------------------------------------------------------
   bool CXplService::CXplMessageFilter::match(const CXplMessage & msg)
   {
      //it's not an heartbeat we signal to the app if it's not filtered
      bool msgCanBeFired = true;
      //if the filter is set and it is different than the value received, we won't fire the event
      if ((!m_msgtype.empty()) && (msg.getTypeIdentifierAsString() != m_msgtype))
         msgCanBeFired = false;

      if ((!m_vendor.empty()) && (msg.getSource().getVendorId() != m_vendor))
         msgCanBeFired = false;

      if ((!m_device.empty()) && (msg.getSource().getDeviceId() != m_device))
         msgCanBeFired = false;

      if ((!m_instance.empty()) && (msg.getSource().getInstanceId() != m_instance))
         msgCanBeFired = false;

      if ((!m_classId.empty()) && (msg.getMessageSchemaIdentifier().getClassId() != m_classId))
         msgCanBeFired = false;

      if ((!m_typeId.empty()) && (msg.getMessageSchemaIdentifier().getTypeId() != m_typeId))
         msgCanBeFired = false;    

      return msgCanBeFired;
   }

   void CXplService::CXplMessageFilter::setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId)
   {
      //We affect the filter value if it's not the wildchar and if it is we set to null the shared_ptr
      if (boost::trim_copy(msgtype) == CXplHelper::WildcardString)
         m_msgtype.clear();
      else
         m_msgtype = boost::trim_copy(msgtype);

      if (boost::trim_copy(vendor) == CXplHelper::WildcardString)
         m_vendor.clear();
      else
         m_vendor = boost::trim_copy(vendor);

      if (boost::trim_copy(device) == CXplHelper::WildcardString)
         m_device.clear();
      else
         m_device = boost::trim_copy(device);

      if (boost::trim_copy(instance) == CXplHelper::WildcardString)
         m_instance.clear();
      else
         m_instance = boost::trim_copy(instance);

      if (boost::trim_copy(classId) == CXplHelper::WildcardString)
         m_classId.clear();
      else
         m_classId = boost::trim_copy(classId);

      if (boost::trim_copy(typeId) == CXplHelper::WildcardString)
         m_typeId.clear();
      else
         m_typeId = boost::trim_copy(typeId);
   }





   void CXplService::subscribeForAllMessages(event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification /*= true*/)
   {
      boost::shared_ptr< CXplMessageFilter > subscribeAll(new CXplMessageFilter(CXplHelper::WildcardString));
      m_filteringSystem.push_back(FilterConfiguration(subscribeAll , pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplService::subscribeForAllMyMessages(event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification /*= true*/)
   {
      boost::shared_ptr< CXplMessageFilter > subscribeAllForMe(new CXplMessageFilter(CXplHelper::WildcardString, m_source.getVendorId(), m_source.getDeviceId(), m_source.getInstanceId(), CXplHelper::WildcardString, CXplHelper::WildcardString));
      m_filteringSystem.push_back(FilterConfiguration(subscribeAllForMe , pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplService::subscribeForMessages(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId, event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification /*= true*/)
   {
      boost::shared_ptr< CXplMessageFilter > filter(new CXplMessageFilter(msgtype, vendor, device, instance, classId, typeId));
      m_filteringSystem.push_back(FilterConfiguration(filter , pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplService::subscribeForMessages(const std::string & filter, event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification /*= true*/)
   {
      boost::shared_ptr< CXplMessageFilter > msgfilter(new CXplMessageFilter(filter));
      m_filteringSystem.push_back(FilterConfiguration(msgfilter , pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplService::clearAllSubscriptions()
   {
      m_filteringSystem.clear();
   }





} } // namespace shared::xpl
