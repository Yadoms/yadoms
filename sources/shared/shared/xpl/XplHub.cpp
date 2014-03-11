#include "stdafx.h"
#include "XplHub.h"

#include "../Log.h"
#include "XplHelper.h"
#include "XplMessage.h"
#include "XplHubConnectedPeripheral.h"
#include "../StringExtension.h"
#include "XplException.h"
#include "../ThreadBase.h"

namespace shared { namespace xpl
{

CXplHub::CXplHub(const std::string & localIPOfTheInterfaceToUse)
   : CThreadBase("XplHub"), m_ioService(), m_socket(m_ioService), m_timer(m_ioService)
{

   if (!CXplHelper::getEndPointFromInterfaceIp(localIPOfTheInterfaceToUse, m_localEndPoint))
   {
      //If we havn't found the given ip, we take the first address IPV4
      m_localEndPoint = CXplHelper::getFirstIPV4AddressEndPoint();
   }
   //we set the listening port
   m_localEndPoint.port(CXplHelper::XplProtocolPort);

   //we configure the socket
   m_socket.open(boost::asio::ip::udp::v4());
   m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
   m_socket.set_option(boost::asio::socket_base::broadcast(true));
   m_socket.bind(m_localEndPoint);

   YADOMS_LOG(info) << "Xpl Hub is starting";
   YADOMS_LOG(info) << "Listening on ip : " << m_localEndPoint.address().to_string() << " on port : " << m_localEndPoint.port();
   m_remoteEndPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), CXplHelper::XplProtocolPort);

   runCheckApplicationLifeCycleTimeout();
   m_stopRequested = false;
   startReceive();
}

CXplHub::~CXplHub()
{
   stop();
}


void CXplHub::startReceive()
{
   m_socket.async_receive(
      boost::asio::buffer(m_receiveBuffer),
      boost::bind(&CXplHub::handleReceive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void CXplHub::doWork()
{
   m_ioService.run();
}

bool CXplHub::stop()
{
   if(!m_ioService.stopped())
      m_ioService.stop();
   return true;
}

void CXplHub::handleReceive(const boost::system::error_code& error,
                            std::size_t bytes_transferred)
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
         YADOMS_LOG(trace) << "Received : " << msg.toString();

         //When the hub receives a hbeat.app or config.app message, the hub should extract the "remote-ip" value 
         //from the message body and compare the IP address with the list of addresses the hub is currently bound 
         //to for the local computer. If the address does not match any local addresses, 
         //the packet moves on to the delivery/rebroadcast step.
         if (CXplMessageSchemaIdentifier::isHeartbeatMessageSchemaIdentifier(msg.getMessageSchemaIdentifier()))
         {
            //we've got an heartbeat we must manage the list
            if (m_localEndPoint.address().to_string() == msg.getBodyValue("remote-ip"))
            {
               YADOMS_LOG(debug) << "Hbeat message";
               //we check if we already known this periph
               unsigned short port;
               if (!CStringExtension::tryParse<unsigned short>(msg.getBodyValue("port"), port))
               {
                  //it is not a hbeat request to be identified but to identify other service, we rebroadcast it

               }
               else
               {
                  int interval;
                  if (!CStringExtension::tryParse<int>(msg.getBodyValue("interval"), interval))
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
                        YADOMS_LOG(debug) << "Update peripheral information";
                        m_discoveredPeripherals[i]->setInterval(interval);
                        m_discoveredPeripherals[i]->updateLastTimeSeenFromNow();
                     }
                     else
                     {
                        //it's a new peripheral, so we add it to the list
                        YADOMS_LOG(debug) << "New peripheral";
                        m_discoveredPeripherals.push_back(boost::shared_ptr<CXplHubConnectedPeripheral>(new CXplHubConnectedPeripheral(m_ioService, m_localEndPoint.address().to_string(), port, interval)));
                     }
                  }
               }
            }
            else
            {
               //From another ip, nothing to do, just rebrodcast it
            }
         }
         else
         {
            //it's not an heartbeat we rebroadcast it
         }

         //we rebrodcast the message to all known equipements
         BOOST_FOREACH(boost::shared_ptr<CXplHubConnectedPeripheral> peripheral, m_discoveredPeripherals)
         {
            peripheral->sendMessage(msg);
         }
      }
      catch (std::exception const& e)
      {
         YADOMS_LOG(error) << e.what() << std::endl;
      }      
   }
   startReceive();
}

void CXplHub::runCheckApplicationLifeCycleTimeout()
{
   m_timer.expires_from_now(boost::posix_time::seconds(ApplicationLifeCycleTimeout));
   m_timer.async_wait(boost::bind(&CXplHub::checkApplicationLifeCycle, this));
}

void CXplHub::checkApplicationLifeCycle()
{
   try
   {
      int i = (int)m_discoveredPeripherals.size() - 1;
      while (i >= 0)
      {
         //we check inactivity using the rule : last seen time + (interval * 2) + 1 minutes < now then periph has died
         boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
         boost::posix_time::ptime timeoutAt = m_discoveredPeripherals[i]->getLastTimeSeen() + boost::posix_time::minutes(m_discoveredPeripherals[i]->getInterval() * 2 + 1);

         if (timeoutAt < now)
         {
            //we must delete the peripheral
            YADOMS_LOG(debug) << "Delete peripheral with port " <<  m_discoveredPeripherals[i]->getPortNumber() << std::endl;
            m_discoveredPeripherals.erase(m_discoveredPeripherals.begin()+i);
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

} } // namespace shared::xpl
