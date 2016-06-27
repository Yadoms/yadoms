#include "stdafx.h"
#include "XplHub.h"

#include "XplHelper.h"
#include "XplMessage.h"
#include "XplHubConnectedPeripheral.h"
#include <shared/StringExtension.h>
#include <shared/ThreadBase.h>
#include <shared/currentTime/Provider.h>


#include <Poco/Net/SocketAddress.h>

#include <Poco/Net/NetException.h>

namespace xplcore
{
   CXplHub::CXplHub(const std::string& localIPOfTheInterfaceToUse)
      : CThreadBase("XplHub"),
        m_socket(Poco::Net::SocketAddress(Poco::Net::IPAddress(), CXplHelper::XplProtocolPort)),
        m_stopHubRequested(false)
   {
      m_socket.setBroadcast(true);
      m_socket.setReuseAddress(true);
   }

   CXplHub::~CXplHub()
   {
      CXplHub::stop();
   }

   void CXplHub::stop()
   {
      m_stopHubRequested = true;
   }

   void CXplHub::doWork()
   {
      try
      {
         runCheckApplicationLifeCycleTimeout();
         while (!m_stopHubRequested) //we don't need locking here - connected is just a boolean
         {
            if (shared::currentTime::Provider().now() > m_nextHeartbeatTime)
            {
               //ensure that if not reconfigure, the next time it do not come here
               m_nextHeartbeatTime = boost::posix_time::max_date_time;
               checkApplicationLifeCycle();
            }
            else
            {
               boost::array<char, 2048> receiveBuffer;

               Poco::Net::SocketAddress sender;
               auto ready = m_socket.poll(Poco::Timespan(1, 0), Poco::Net::Socket::SELECT_READ);
               if (!ready)
               {
                  continue;
               }
               auto bytesRead = m_socket.receiveFrom(receiveBuffer.c_array(), 2048, sender);
               if (bytesRead == 0)
               {
                  continue;
               }

               // Create an XplMsg object from the received data
               std::string data;
               std::copy(receiveBuffer.begin(), receiveBuffer.begin() + bytesRead, std::back_inserter(data));

               auto msg = CXplMessage::parse(data);
               onXplMessageReceived(msg, sender);
            }

            boost::this_thread::sleep(boost::posix_time::milliseconds(100)); //allow thread interrupt
         }
      }
      catch (boost::thread_interrupted&)
      {
      }
      catch (shared::exception::CException& ex)
      {
         std::cerr << "The XPL hub fails. Unknown expcetion : " << ex.what() << std::endl;
      }
      std::cout << "Xpl Hub ends" << std::endl;
   }


   void CXplHub::onXplMessageReceived(CXplMessage& msg,
                                      Poco::Net::SocketAddress& sender)
   {
      //the message is successfully parsed
      std::cout << "Received From : " << msg.getSource().toString() << "  message= " << msg.getMessageSchemaIdentifier().toString() << std::endl;

      if (CXplMessageSchemaIdentifier::isHeartbeatApp(msg.getMessageSchemaIdentifier()))
      {
         //we've got an heartbeat we must manage the list
         //check if the remote-ip inckluded in the message, match on of local ip
         manageReceivedHeartbeatMessage(msg, sender);
      }
      //we rebrodcast the message to all known equipements
      broadcastMessage(msg);
   }


   void CXplHub::manageReceivedHeartbeatMessage(CXplMessage& hbeatMessage,
                                                Poco::Net::SocketAddress& sender)
   {
      //When the hub receives a hbeat.app or config.app message, the hub should extract the "remote-ip" value 
      //from the message body and compare the IP address with the list of addresses the hub is currently bound 
      //to for the local computer. If the address does not match any local addresses, 
      //the packet moves on to the delivery/rebroadcast step.
      //TODO : manage the "config.app" message (from above comment)

      std::cout << "Heartbeat message received" << std::endl;
      //we check if we already known this periph
      unsigned short port;
      if (!shared::CStringExtension::tryParse<unsigned short>(hbeatMessage.getBodyValue("port"), port))
      {
         //it is not a hbeat request to be identified but to identify other service, we rebroadcast it
      }
      else
      {
         int interval;
         if (!shared::CStringExtension::tryParse<int>(hbeatMessage.getBodyValue("interval"), interval))
         {
            //it is not a hbeat request to be identified but to identify other service, we rebroadcast it
         }
         else
         {
            size_t i = 0;

            while ((i < m_discoveredPeripherals.size()) && (port != m_discoveredPeripherals[i]->getPortNumber()))
            {
               i++;
            }

            if (i < m_discoveredPeripherals.size())
            {
               //we already known the peripheral, so we update its interval and its last time seen
               std::cout << "Update peripheral information" << std::endl;
               m_discoveredPeripherals[i]->setInterval(interval);
               m_discoveredPeripherals[i]->updateLastTimeSeenFromNow();
            }
            else
            {
               try
               {
                  //it's a new peripheral, so we add it to the list
                  std::cout << "New peripheral" << std::endl;
                  m_discoveredPeripherals.push_back(boost::make_shared<CXplHubConnectedPeripheral>(sender,
                                                                                                   port,
                                                                                                   interval,
                                                                                                   hbeatMessage.getSource().toString()));
               }
               catch (std::exception& ex)
               {
                  //it's a new peripheral, so we add it to the list
                  std::cerr << "Fail to add peripheral : " << ex.what() << std::endl;
               }
            }
         }
      }
   }


   void CXplHub::broadcastMessage(CXplMessage& msg)
   {
      for (auto i = m_discoveredPeripherals.begin(); i != m_discoveredPeripherals.end(); ++i)
      {
         try
         {
            (*i)->sendMessage(msg);
         }
         catch (Poco::Net::NetException& netex)
         {
            //it's a new peripheral, so we add it to the list
            std::cerr << "broadcastMessage failed : " << netex.message() << std::endl;
            std::cerr << netex.displayText() << std::endl;
         }
         catch (std::exception& ex)
         {
            std::cerr << "broadcastMessage failed : " << ex.what() << std::endl;
         }
         catch (...)
         {
            std::cerr << "broadcastMessage failed : unknown exception" << std::endl;
         }
      }
   }


   void CXplHub::runCheckApplicationLifeCycleTimeout()
   {
      m_nextHeartbeatTime = shared::currentTime::Provider().now() + boost::posix_time::seconds(ApplicationLifeCycleTimeout);
   }

   void CXplHub::checkApplicationLifeCycle()
   {
      try
      {
         auto i = static_cast<int>(m_discoveredPeripherals.size()) - 1;
         while (i >= 0)
         {
            //we check inactivity using the rule : last seen time + (interval * 2) + 1 minutes < now then periph has died
            auto timeoutAt = m_discoveredPeripherals[i]->getLastTimeSeen() + boost::posix_time::minutes(m_discoveredPeripherals[i]->getInterval() * 2 + 1);

            if (timeoutAt < shared::currentTime::Provider().now())
            {
               //we must delete the peripheral
               std::cout << "Delete peripheral with port " << m_discoveredPeripherals[i]->getPortNumber() << std::endl;
               m_discoveredPeripherals.erase(m_discoveredPeripherals.begin() + i);
            }
            i--;
         }
      }
      catch (std::exception const& e)
      {
         std::cerr << e.what() << std::endl;
      }

      //we relaunch the check in one minute
      runCheckApplicationLifeCycleTimeout();
   }
} // namespace xplcore


