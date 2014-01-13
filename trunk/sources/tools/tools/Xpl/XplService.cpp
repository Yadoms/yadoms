#include "stdafx.h"
#include "XplService.h"

#include "Log.h"
#include "XplException.h"
#include "XplMessage.h"
#include "XplMessageFactory.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "StringExtension.hpp"

//un client broadcast sa donnée sur le port XPL
//et passe en ecoute de données sur n'importe quel port de n'importe qui qu'il donne dans sa trame xpl

//un hub ecoute le port xpl
//en repete les données sur la liste des client connectés sur leur port respectifs


CXplService::CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId)
   : m_ioService(), m_timer(m_ioService), m_socket(m_ioService)
{
   m_localEndPoint = CXplHelper::getFirstIPV4AddressEndPoint();
   m_source = CXplActor::createActor(vendorId, deviceId, instanceId);
   initializeConnector();
}

CXplService::CXplService(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId, const std::string & localIPOfTheInterfaceToUse)
   : m_ioService(), m_timer(m_ioService), m_socket(m_ioService)
{
   if (!CXplHelper::getEndPointFromInterfaceIp(localIPOfTheInterfaceToUse, m_localEndPoint))
   {
      //If we havn't found the given ip, we take the first address IPV4
      m_localEndPoint = CXplHelper::getFirstIPV4AddressEndPoint();
   }

   m_source = CXplActor::createActor(vendorId, deviceId, instanceId);
   initializeConnector();
}

CXplService::~CXplService()
{
}

void CXplService::initializeConnector()
{
   m_hubHasBeenFound = false;
   m_startDate = boost::posix_time::second_clock::local_time();

   runHeartbeatSequenceIn(HeartbeatFrequencyDuringInitialDiscoveryPhase);

   //we configure the socket
   m_socket.open(boost::asio::ip::udp::v4());
   m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
   m_socket.set_option(boost::asio::socket_base::broadcast(true));
   m_socket.bind(m_localEndPoint);

   m_remoteEndPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::broadcast(), CXplHelper::XplProtocolPort);

   boost::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));

   m_socket.async_receive(
         boost::asio::buffer(m_receiveBuffer),
         boost::bind(&CXplService::handleReceive, this,
         boost::asio::placeholders::error,
         boost::asio::placeholders::bytes_transferred));
}

void CXplService::runHeartbeatSequenceIn(const int seconds)
{
   m_timer.expires_from_now(boost::posix_time::seconds(seconds));
   m_timer.async_wait(boost::bind(&CXplService::heartbeatSequence, this));
}

void CXplService::heartbeatSequence()
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
      boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
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
   CXplMessage msg = CXplMessageFactory::createHeartbeatMessage(m_source, HeartbeatInterval, m_socket.local_endpoint().address().to_string(), m_socket.local_endpoint().port());
   std::cout << msg.toString() << std::endl;

   m_socket.send_to(boost::asio::buffer(msg.toString()), m_remoteEndPoint);

   //We relaunch the hbeat
   runHeartbeatSequenceIn(heartbeatInterval);
}

void CXplService::handleReceive(const boost::system::error_code& error,
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
         YADOMS_LOG(debug) << "Message received : " << msg.toString();
         //When the hub receives a hbeat.app or config.app message, the hub should extract the "remote-ip" value 
         //from the message body and compare the IP address with the list of addresses the hub is currently bound 
         //to for the local computer. If the address does not match any local addresses, 
         //the packet moves on to the delivery/rebroadcast step.
         if (CXplMessageSchemaIdentifier::isHeartbeatMessageSchemaIdentifier(msg.getMessageSchemaIdentifier()))
         {
            //we've got an heartbeat if it's our hbeat and we are m_hubHasBeenFound = false we have found a hub
            int port;
            if (!CStringExtension::tryParse<int>(msg.getBodyValue("port"), port))
               throw CXplException("port in Heartbeat message is incorrect : " + msg.toString());

            if ((!m_hubHasBeenFound) && (m_localEndPoint.address().to_string() == msg.getBodyValue("remote-ip")) && (m_socket.local_endpoint().port() == port))
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
            //it's not an heartbeat we signal to the app if it's not filtered
            bool msgCanBeFired = true;
            //if the filter is set and it is different than the value received, we won't fire the event
            if ((m_filter.msgtype) && (msg.getTypeIdentifierAsString() != *m_filter.msgtype.get()))
               msgCanBeFired = false;

            if ((m_filter.vendor) && (msg.getSource().getVendorId() != *m_filter.vendor.get()))
               msgCanBeFired = false;

            if ((m_filter.device) && (msg.getSource().getDeviceId() != *m_filter.device.get()))
               msgCanBeFired = false;

            if ((m_filter.instance) && (msg.getSource().getInstanceId() != *m_filter.instance.get()))
               msgCanBeFired = false;

            if ((m_filter.classId) && (msg.getMessageSchemaIdentifier().getClassId() != *m_filter.classId.get()))
               msgCanBeFired = false;

            if ((m_filter.typeId) && (msg.getMessageSchemaIdentifier().getTypeId() != *m_filter.typeId.get()))
               msgCanBeFired = false;

            //We fire the message only if it is not filtered
            if (msgCanBeFired)
            {
               YADOMS_LOG(debug) << "Message not filtered";
               fireMessageReceivedEvent(msg);
            }
            else
            {
               YADOMS_LOG(debug) << "Message filtered";
            }
         }
      }
      catch (std::exception const& e)
      {
         std::cerr << e.what() << std::endl;
      }      
   }
   m_socket.async_receive(
      boost::asio::buffer(m_receiveBuffer),
      boost::bind(&CXplService::handleReceive, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void CXplService::sendMessage(const CXplMessage & message)
{
   m_socket.send_to(boost::asio::buffer(message.toString()), m_remoteEndPoint);
}

void CXplService::messageReceived(const SigMessageReceivedDelegate &dlg)
{
   m_sigMessageReceived.connect(dlg);
}

void CXplService::fireMessageReceivedEvent(CXplMessage & msg)
{
    if (!m_sigMessageReceived.empty()) 
      m_sigMessageReceived(boost::ref(msg));
}

void CXplService::setFilter(const std::string & filter)
{
   std::string lineString = boost::trim_copy(filter);

   //we allow a single "*" for all filter instead of *.*.*.*.*.*
   if (lineString == CXplHelper::WildcardString)
   {
      //we clear all filters
      m_filter.msgtype.reset();
      m_filter.vendor.reset();
      m_filter.device.reset();
      m_filter.instance.reset();
      m_filter.classId.reset();
      m_filter.typeId.reset();
   }
   
   std::vector<std::string> line;
   boost::split(line, lineString, boost::is_any_of("."), boost::token_compress_on);
   if (line.size() != 6)
      throw CXplException("The filter string must be formed like : [msgtype].[vendor].[device].[instance].[class].[type] (wildchar * is allowed). Value given : " + filter);

   setFilter(line[0], line[1], line[2], line[3], line[4], line[5]);
}

void CXplService::setFilter(const std::string & msgtype, const std::string & vendor, const std::string & device, const std::string & instance, 
               const std::string & classId, const std::string & typeId)
{
   //We affect the filter value if it's not the wildchar and if it is we set to null the shared_ptr
   if (boost::trim_copy(msgtype) == CXplHelper::WildcardString)
      m_filter.msgtype.reset();
   else
      m_filter.msgtype = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(msgtype)));

   if (boost::trim_copy(vendor) == CXplHelper::WildcardString)
      m_filter.vendor.reset();
   else
      m_filter.vendor = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(vendor)));

   if (boost::trim_copy(device) == CXplHelper::WildcardString)
      m_filter.device.reset();
   else
      m_filter.device = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(device)));

   if (boost::trim_copy(instance) == CXplHelper::WildcardString)
      m_filter.instance.reset();
   else
      m_filter.instance = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(instance)));

   if (boost::trim_copy(classId) == CXplHelper::WildcardString)
      m_filter.classId.reset();
   else
      m_filter.classId = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(classId)));

   if (boost::trim_copy(typeId) == CXplHelper::WildcardString)
      m_filter.typeId.reset();
   else
      m_filter.typeId = boost::shared_ptr<std::string>(new std::string(boost::trim_copy(typeId)));
}

CXplActor CXplService::getActor() const
{
   return m_source;
}