#include "stdafx.h"
#include "XplServiceTask.h"

#include <shared/Log.h>
#include "XplException.h"
#include "XplMessage.h"
#include "XplMessageFactory.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplConstants.h"
#include <shared/StringExtension.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#include <Poco/Net/SocketAddress.h>

#include <Poco/RunnableAdapter.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/NetException.h>
#include <Poco/Runnable.h>
#include <Poco/Buffer.h>


// A client send its data as broadcast on the XPL port,
// and listen on a certain port, given in its XPL frame.

// A hub is listening the XPL port, repeat all received data to the connected clients on their respective ports.

namespace xplcore
{

   CXplServiceTask::CXplServiceTask(Poco::Net::NetworkInterface & networkInterface , const std::string & vendorId, const std::string & deviceId, const std::string & instanceId,
      shared::event::CEventHandler * pHubFoundEventHandler, int hubFoundEventCode)
      : Poco::Task(networkInterface.address().toString()), m_pHubFoundEventHandler(pHubFoundEventHandler), m_hubFoundEventCode(hubFoundEventCode)
   {
      m_source = CXplActor::createActor(vendorId, deviceId, instanceId);
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


      void CXplServiceTask::runTask()   {
      try
      {
         YADOMS_LOG_CONFIGURE(m_source.toString());
         YADOMS_LOG(information) << "XplServiceTask : run";
         
         runHeartbeatSequenceIn(HeartbeatFrequencyDuringInitialDiscoveryPhase);

         while (!isCancelled()) //we don't need locking here - connected is just a boolean
         {
            if (boost::posix_time::second_clock::universal_time() > m_nextHeartbeatTime)
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
         YADOMS_LOG(fatal) << "The XplServiceTask fails. Unknown expcetion : " << ex.what();
      }
   }

   void CXplServiceTask::initializeConnector(Poco::Net::NetworkInterface & networkInterface)
   {
      m_hubHasBeenFound = false;
      m_startDate = boost::posix_time::second_clock::universal_time();

      //the localendpoint is determined from the network interface 
      //(take the ip from the interface and get an used port between 49152 and 65535 according to xpl documentation)
      boost::random::mt19937 gen;
      boost::random::uniform_int_distribution<> dist(49152, 65535);
      m_localEndPoint = Poco::Net::SocketAddress(networkInterface.address(), dist(gen));

      //the remote interface is just a broadcast one on xpl port
      m_remoteEndPoint = Poco::Net::SocketAddress(Poco::Net::IPAddress::broadcast(), CXplHelper::XplProtocolPort);

      //we configure the socket
      m_socket.setReuseAddress(true);
      m_socket.setBroadcast(true);
      m_socket.bind(m_localEndPoint, true);

      YADOMS_LOG(debug) << "CXplService : Local EndPoint: " << m_localEndPoint.toString() << " on port : " << m_localEndPoint.port();
      YADOMS_LOG(debug) << "CXplService : Remote EndPoint: " << m_remoteEndPoint.toString() << " on port : " << m_remoteEndPoint.port();
   }



   void CXplServiceTask::runHeartbeatSequenceIn(const int seconds)
   {
      boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
      m_nextHeartbeatTime = now + boost::posix_time::seconds(seconds);
   }

   void CXplServiceTask::heartbeatSequence()
   {
      try
      {
         YADOMS_LOG(debug) << "Send heartbeat";

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


         //We send the hbeat
         //we set always the interval of HeartbeatInterval in the message
         CXplMessage msg = CXplMessageFactory::createHeartbeatAppMessage(m_source, HeartbeatInterval, m_localEndPoint.host().toString(), m_localEndPoint.port());
         sendMessage(msg);

         //We relaunch the hbeat
         runHeartbeatSequenceIn(heartbeatInterval);
      }
      catch (Poco::Net::NetException & netex)
      {
         YADOMS_LOG(fatal) << "Sending Message fail. Net Exception : " << netex.what()
            << std::endl << netex.displayText()
            << std::endl << netex.message();
      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(error) << "Send heartbeat fail. Exception : " << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Send heartbeat failed, unkonown exception";
      }
   }





   void CXplServiceTask::onXplMessageReceived(CXplMessage & msg, Poco::Net::SocketAddress & sender)
   {

      //the message is successfully parsed
      YADOMS_LOG(trace) << "Message received : " << msg.toString();

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
               YADOMS_LOG(information) << "Hub found on network : " << m_localEndPoint.host().toString();
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
         YADOMS_LOG(debug) << "Message filtered";
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
         YADOMS_LOG(fatal) << "Sending Message fail. Net Exception : " << netex.what() 
                           << std::endl  << netex.displayText() 
                           << std::endl << netex.message();

         YADOMS_LOG(fatal) << "Remote endpoint : " << m_remoteEndPoint.toString();

      }      
      catch (std::exception & ex)
      {
         YADOMS_LOG(fatal) << "Sending Message fail. Exception : " << ex.what();
      }
      catch (...)
      {
         YADOMS_LOG(fatal) << "Sending Message fail. Unknown exception";
      }

   }



} // namespace xplcore
