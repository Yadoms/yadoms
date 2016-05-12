#include "stdafx.h"
#include "XplServiceTask.h"

#include "XplMessage.h"
#include "XplMessageFactory.h"
#include "XplActor.h"
#include "XplHelper.h"
#include <shared/StringExtension.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#include <Poco/Net/SocketAddress.h>

#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/NetException.h>


// A client send its data as broadcast on the XPL port,
// and listen on a certain port, given in its XPL frame.

// A hub is listening the XPL port, repeat all received data to the connected clients on their respective ports.

namespace xplcore
{

   CXplServiceTask::CXplServiceTask(Poco::Net::NetworkInterface & networkInterface , const std::string & vendorId, const std::string & deviceId, const std::string & instanceId,
      shared::event::CEventHandler * pHubFoundEventHandler, int hubFoundEventCode)
      : Poco::Task(networkInterface.address().toString()), m_pHubFoundEventHandler(pHubFoundEventHandler), m_hubFoundEventCode(hubFoundEventCode), m_source(CXplActor::createActor(vendorId, deviceId, instanceId))
   {
      initializeConnector(networkInterface);
   }

   CXplServiceTask::~CXplServiceTask()
   {
      m_socket.close();
   }

   CXplActor CXplServiceTask::getActor() const
   {
      return m_source;
   }


      void CXplServiceTask::runTask()
   {
      try
      {
         std::cout << "XplServiceTask : run" << std::endl;
         
         runHeartbeatSequenceIn(HeartbeatFrequencyDuringInitialDiscoveryPhase);

         while (!isCancelled()) //we don't need locking here - connected is just a boolean
         {
            if (shared::currentTime::Provider::now() > m_nextHeartbeatTime)
            {
               //ensure that if not reconfigure, the next time it do not come here
               m_nextHeartbeatTime = boost::posix_time::max_date_time;
               heartbeatSequence();
            }
            else
            {
               boost::array<char, 2048> receiveBuffer;
               Poco::Net::SocketAddress sender;
               bool ready = m_socket.poll(Poco::Timespan(1, 0), Poco::Net::Socket::SELECT_READ);
               if (!ready)
               {
                  continue;
               }
               int bytesRead = m_socket.receiveFrom(receiveBuffer.c_array(), 2048, sender);
               if (bytesRead == 0)
               {
                  continue;
               }

               // Create an XplMsg object from the received data
               std::string data;
               std::copy(receiveBuffer.begin(), receiveBuffer.begin() + bytesRead, std::back_inserter(data));

               CXplMessage msg = CXplMessage::parse(data);
               onXplMessageReceived(msg, sender);
            }
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
      catch (shared::exception::CException & ex)
      {
         std::cerr << "The XplServiceTask fails. Unknown expcetion : " << ex.what();
      }

      std::cout << "XplServiceTask : stopped" << std::endl;
   }

   void CXplServiceTask::initializeConnector(Poco::Net::NetworkInterface & networkInterface)
   {
      m_hubHasBeenFound = false;
      m_startDate = shared::currentTime::Provider::now();

      //the localendpoint is determined from the network interface 
      //(take the ip from the interface and get an used port between 49152 and 65535 according to xpl documentation)
      boost::random::mt19937 gen;
      boost::random::uniform_int_distribution<> dist(49152, 65535);

      //because our socket only supports ipv4; we need to get the first ipv4 address
      const Poco::Net::IPAddress & firstIpV4Addr = networkInterface.firstAddress(Poco::Net::IPAddress::Family::IPv4);
      m_localEndPoint = Poco::Net::SocketAddress(firstIpV4Addr, (Poco::UInt16)dist(gen)); //cast allowed because value is in [49152, 65535]

      //the remote interface is just a broadcast one on xpl port
      m_remoteEndPoint = Poco::Net::SocketAddress(Poco::Net::IPAddress::broadcast(), CXplHelper::XplProtocolPort);

      //we configure the socket
      std::cout << "CXplService : Remote EndPoint: " << m_remoteEndPoint.toString() << " on port : " << m_remoteEndPoint.port() << std::endl;
      std::cout << "CXplService : Try to bind local endPoint: " << m_localEndPoint.toString() << " on port : " << m_localEndPoint.port() << std::endl;
      m_socket.setReuseAddress(true);
      m_socket.setBroadcast(true);
      m_socket.bind(m_localEndPoint, true);
   }



   void CXplServiceTask::runHeartbeatSequenceIn(const int seconds)
   {
      m_nextHeartbeatTime = shared::currentTime::Provider::now() + boost::posix_time::seconds(seconds);
   }

   void CXplServiceTask::heartbeatSequence()
   {
      try
      {
         std::cout << "Send heartbeat" << std::endl;

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
            boost::posix_time::time_duration diff = shared::currentTime::Provider::now() - m_startDate;
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


         //We send the hbeat
         //we set always the interval of HeartbeatInterval in the message
         CXplMessage msg = CXplMessageFactory::createHeartbeatAppMessage(m_source, HeartbeatInterval, m_localEndPoint.host().toString(), m_localEndPoint.port());
         sendMessage(msg);

         //We relaunch the hbeat
         runHeartbeatSequenceIn(heartbeatInterval);
      }
      catch (Poco::Net::NetException & netex)
      {
         std::cerr << "Sending Message fail. Net Exception : " << netex.what()
            << std::endl << netex.displayText()
            << std::endl << netex.message()
            << std::endl;
      }
      catch (std::exception & ex)
      {
         std::cerr << "Send heartbeat fail. Exception : " << ex.what() << std::endl;
      }
      catch (...)
      {
         std::cerr << "Send heartbeat failed, unkonown exception" << std::endl;
      }
   }





   void CXplServiceTask::onXplMessageReceived(CXplMessage & msg, Poco::Net::SocketAddress & sender)
   {

      //the message is successfully parsed
      std::cout << "Message received : " << msg.toString() << std::endl;

      //When the hub receives a hbeat.app or config.app message, the hub should extract the "remote-ip" value 
      //from the message body and compare the IP address with the list of addresses the hub is currently bound 
      //to for the local computer. If the address does not match any local addresses, 
      //the packet moves on to the delivery/rebroadcast step.
      if (CXplMessageSchemaIdentifier::isHeartbeatMessageSchemaIdentifier(msg.getMessageSchemaIdentifier()))
      {
         //we've got an heartbeat we must manage the list
         //check if the remote-ip inckluded in the message, match on of local ip
         manageReceivedHeartBeatMessage(msg, sender);
      }
      else
      {
         //it is a normal message, forward notification to subscribers
         notifySubscribers(msg);
      }
   }



   void CXplServiceTask::manageReceivedHeartBeatMessage(CXplMessage & hbeatMessage, Poco::Net::SocketAddress & sender)
   {
      if (CXplMessageSchemaIdentifier::isHeartbeatApp(hbeatMessage.getMessageSchemaIdentifier()))
      {
         //we've got an heartbeat if it's our hbeat and we are m_hubHasBeenFound = false we have found a hub
         int port;
         if (shared::CStringExtension::tryParse<int>(hbeatMessage.getBodyValue("port"), port))
         {
            if ((!m_hubHasBeenFound) && (m_localEndPoint.host().toString() == hbeatMessage.getBodyValue("remote-ip")) && (sender.port() == port))
            {
               std::cout << "Hub found on network : " << m_localEndPoint.host().toString() << std::endl;
               m_hubHasBeenFound = true;

               if (m_pHubFoundEventHandler != NULL)
                  m_pHubFoundEventHandler->postEvent(m_hubFoundEventCode);
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
      else if (CXplMessageSchemaIdentifier::isHeartbeatRequest(hbeatMessage.getMessageSchemaIdentifier()))
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


   void CXplServiceTask::subscribeForAllMessages(shared::event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification)
   {
      boost::shared_ptr< CXplMessageFilter > subscribeAll(new CXplMessageFilter(CXplHelper::WildcardString));
      m_filteringSystem.push_back(FilterConfiguration(subscribeAll, pEventHandler, eventTypeIdentifier, continueChainingNotification));

   }

   void CXplServiceTask::subscribeForAllMyMessages(shared::event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification)
   {
      boost::shared_ptr< CXplMessageFilter > subscribeAllForMe(new CXplMessageFilter(CXplHelper::WildcardString, m_source.getVendorId(), m_source.getDeviceId(), m_source.getInstanceId(), CXplHelper::WildcardString, CXplHelper::WildcardString));
      m_filteringSystem.push_back(FilterConfiguration(subscribeAllForMe, pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplServiceTask::subscribeForMessages(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, const std::string & classId, const std::string & typeId, shared::event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification)
   {
      boost::shared_ptr< CXplMessageFilter > filter(new CXplMessageFilter(msgtype, vendor, device, instance, classId, typeId));
      m_filteringSystem.push_back(FilterConfiguration(filter, pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }

   void CXplServiceTask::subscribeForMessages(const std::string & filter, shared::event::CEventHandler * pEventHandler, int eventTypeIdentifier, bool continueChainingNotification)
   {
      boost::shared_ptr< CXplMessageFilter > msgfilter(new CXplMessageFilter(filter));
      m_filteringSystem.push_back(FilterConfiguration(msgfilter, pEventHandler, eventTypeIdentifier, continueChainingNotification));
   }


   void CXplServiceTask::clearAllSubscriptions()
   {
      m_filteringSystem.clear();
   }


   //--------------------------------------------------------------
   /// \brief			Send notification to all configured subscribers
   /// \param [in]   message : The message to notify
   //--------------------------------------------------------------
   void CXplServiceTask::notifySubscribers(CXplMessage & msg)
   {
      bool atLeastOneNotificationSend = false;

      for (std::vector< FilterConfiguration >::iterator i = m_filteringSystem.begin(); i != m_filteringSystem.end(); ++i)
      {
         boost::shared_ptr< CXplMessageFilter > filter = i->get<0>();
         if (filter && filter->match(msg))
         {
            shared::event::CEventHandler * pEventHandler = i->get<1>();
            if (pEventHandler)
            {
               //send notification
               pEventHandler->postEvent<CXplMessage>(i->get<2>(), boost::ref(msg));

               //
               atLeastOneNotificationSend = true;

               //check for notification chaining
               bool continueNotificationChaining = i->get<3>();

               //if do not continue chaing, just break the foreach
               if (!continueNotificationChaining)
                  break;
            }
         }
      }

      if (!atLeastOneNotificationSend)
      {
         std::cout << "Message filtered" << std::endl;
      }
   }


   void CXplServiceTask::sendMessage(const CXplMessage & msg)
   {
      try
      {
         //bufferize data
         std::string buf = msg.toString();

         //lock socket access
         boost::lock_guard<boost::mutex> lock(m_socketLocker);

         //send message
         m_socket.sendTo(buf.c_str(), buf.size(), m_remoteEndPoint);
      }
      catch (Poco::Net::NetException & netex)
      {
         std::cerr << "Sending Message fail. Net Exception : " << netex.what() 
                           << std::endl  << netex.displayText() 
                           << std::endl << netex.message()
                           << std::endl;

         std::cerr << "Remote endpoint : " << m_remoteEndPoint.toString() << std::endl;

      }      
      catch (std::exception & ex)
      {
         std::cerr << "Sending Message fail. Exception : " << ex.what() << std::endl;
      }
      catch (...)
      {
         std::cerr << "Sending Message fail. Unknown exception" << std::endl;
      }

   }



} // namespace xplcore
