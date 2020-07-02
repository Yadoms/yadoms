#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/http/ssdp/DiscoveredDevice.h"
#include <shared/Log.h>

BOOST_AUTO_TEST_SUITE(TestDiscoveredDevice)

   BOOST_AUTO_TEST_CASE(InvalidContruction)
   {
      BOOST_CHECK_THROW(shared::http::ssdp::CDiscoveredDevice dd(""), std::runtime_error);
   }

   BOOST_AUTO_TEST_CASE(Lametric)
   {
      static const auto ssdpAnswer = std::string(
         "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
         "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">"
         "<specVersion>"
         "<major>1</major>"
         "<minor>0</minor>"
         "</specVersion>"
         "<URLBase>https://192.168.1.10:443</URLBase>"
         "<device>"
         "<deviceType>urn:schemas-upnp-org:device:LaMetric:1</deviceType>"
         "<friendlyName>LaMetric Time (LM3989)</friendlyName>"
         "<manufacturer>LaMetric Inc.</manufacturer>"
         "<manufacturerURL>http://www.lametric.com</manufacturerURL>"
         "<modelDescription>LaMetric - internet connected clock and smart display</modelDescription>"
         "<modelName>LaMetric Time</modelName>"
         "<modelNumber>SA01</modelNumber>"
         "<modelURL>http://www.lametric.com</modelURL>"
         "<serialNumber>SA171202398900W00BS9</serialNumber>"
         "<serverId>27539</serverId>"
         "<deviceId>27539</deviceId>"
         "<UDN>uuid:ca964fe6-117e-4880-a303-3b0c13c125fc</UDN>"
         "</device>"
         "</root>");

      const shared::http::ssdp::CDiscoveredDevice dd(ssdpAnswer);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.URLBase"), "https://192.168.1.10:443");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.deviceType"),
                        "urn:schemas-upnp-org:device:LaMetric:1");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.friendlyName"), "LaMetric Time (LM3989)");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturer"), "LaMetric Inc.");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturerURL"), "http://www.lametric.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelDescription"),
                        "LaMetric - internet connected clock and smart display");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelName"), "LaMetric Time");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelNumber"), "SA01");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelURL"), "http://www.lametric.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serialNumber"), "SA171202398900W00BS9");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<int>("root.device.serverId"), 27539);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<int>("root.device.deviceId"), 27539);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UDN"), "uuid:ca964fe6-117e-4880-a303-3b0c13c125fc");
   }

   BOOST_AUTO_TEST_CASE(SynologyDS216IIPlusSsdp)
   {
      static const auto ssdpAnswer = std::string(
         "<?xml version=\"1.0\"?>\n<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n\t<specVersion>\n\t\t<major>1</major>\n\t\t<minor>0</minor>\n\t</specVersion>\n\t<device>\n\t\t<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>\n\t\t<friendlyName>NAS (DS216+II)</friendlyName>\n\t\t<manufacturer>Synology</manufacturer>\n\t\t<manufacturerURL>http://www.synology.com</manufacturerURL>\n\t\t<modelDescription>Synology NAS</modelDescription>\n\t\t<modelName>DS216+II</modelName>\n\t\t<modelNumber>DS216+II 6.2-25426</modelNumber>\n\t\t<modelURL>http://www.synology.com</modelURL>\n\t\t<modelType>NAS</modelType>\n\t\t<serialNumber>0011325e4496</serialNumber>\n\t\t<UDN>uuid:73796E6F-6473-6D00-0000-0011325e4496</UDN>\n\t\t<serviceList>\n\t\t\t<service>\n\t\t\t\t<URLBase>http://192.168.0.2:5000</URLBase>\n\t\t\t\t<serviceType>urn:schemas-dummy-com:service:Dummy:1</serviceType>\n\t\t\t\t<serviceId>urn:dummy-com:serviceId:dummy1</serviceId>\n\t\t\t\t<controlURL>/dummy</controlURL>\n\t\t\t\t<eventSubURL>/dummy</eventSubURL>\n\t\t\t\t<SCPDURL>/ssdp/dummy.xml</SCPDURL>\n\t\t\t</service>\n\t\t</serviceList>\n\t\t<presentationURL>http://192.168.0.2:5000/</presentationURL>\n\t</device>\n</root>\n\n");

      const shared::http::ssdp::CDiscoveredDevice dd(ssdpAnswer);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serviceList.service.URLBase"), "http://192.168.0.2:5000");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.deviceType"),
                        "urn:schemas-upnp-org:device:Basic:1");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.friendlyName"), "NAS (DS216+II)");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturer"), "Synology");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturerURL"), "http://www.synology.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelDescription"), "Synology NAS");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelName"), "DS216+II");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelNumber"), "DS216+II 6.2-25426");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelURL"), "http://www.synology.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelType"), "NAS");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serialNumber"), "0011325e4496");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UDN"), "uuid:73796E6F-6473-6D00-0000-0011325e4496");
   }

   BOOST_AUTO_TEST_CASE(SynologyDS216IIPlusDesc)
   {
      static const auto ssdpAnswer = std::string(
         "<?xml version=\"1.0\"?>\n<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n<specVersion>\n\t<major>1</major>\n\t<minor>0</minor>\n</specVersion>\n<device>\n\t<deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>\n\t<friendlyName>NAS</friendlyName>\n\t<manufacturer>Synology Inc.</manufacturer>\n\t<manufacturerURL>http://www.synology.com/</manufacturerURL>\n\t<modelName>DS216+II</modelName>\n\t<modelDescription>Synology DLNA/UPnP Media Server</modelDescription>\n\t<modelURL>http://www.synology.com</modelURL>\n\t<serialNumber>1650O6N558101</serialNumber>\n\t<UDN>uuid:0011325e-4496-0011-9644-96445e321100</UDN>\n\t<presentationURL>http://192.168.0.2:5000/</presentationURL>\n    <iconList>\n        <icon>\n            <mimetype>image/jpeg</mimetype>\n            <width>120</width>\n            <height>120</height>\n            <depth>24</depth>\n            <url>/tmp_icon/dmsicon120.jpg</url>\n        </icon>\n        <icon>\n            <mimetype>image/jpeg</mimetype>\n            <width>48</width>\n            <height>48</height>\n            <depth>24</depth>\n            <url>/tmp_icon/dmsicon48.jpg</url>\n        </icon> \n        <icon>\n            <mimetype>image/png</mimetype>\n            <width>120</width>\n            <height>120</height>\n            <depth>24</depth>\n            <url>/tmp_icon/dmsicon120.png</url>\n        </icon>\n        <icon>\n            <mimetype>image/png</mimetype>\n            <width>48</width>\n            <height>48</height>\n            <depth>24</depth>\n            <url>/tmp_icon/dmsicon48.png</url>\n        </icon> \n\n    </iconList>\n\n\t<serviceList>\n\t\t<service>\n\t\t\t<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>\n\t\t\t<serviceId>urn:upnp-org:serviceId:ConnectionManager</serviceId>\n\t\t\t<SCPDURL>/connmgrSCPD.xml</SCPDURL>\n\t\t\t<controlURL>/ConnectionManager/control</controlURL>\n\t\t\t<eventSubURL>/ConnectionManager/event</eventSubURL>\n\t\t</service>\n\t\t<service>\n\t\t\t<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>\n\t\t\t<serviceId>urn:upnp-org:serviceId:ContentDirectory</serviceId>\n\t\t\t<SCPDURL>/cdsxSCPD.xml</SCPDURL>\n\t\t\t<controlURL>/ContentDirectory/control</controlURL>\n\t\t\t<eventSubURL>/ContentDirectory/event</eventSubURL>\n\t\t</service>\n\t\t<service>\n\t\t\t<serviceType>urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1</serviceType>\n\t\t\t<serviceId>urn:microsoft.com:serviceId:X_MS_MediaReceiverRegistrar</serviceId>\n\t\t\t<SCPDURL>/msrrSCPD.xml</SCPDURL>\n\t\t\t<eventSubURL>/MediaReceiverRegistrar/event</eventSubURL>\n\t\t\t<controlURL>/MediaReceiverRegistrar/control</controlURL>\n\t\t</service>\n\t</serviceList>\n</device>\n</root>\n");

      const shared::http::ssdp::CDiscoveredDevice dd(ssdpAnswer);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.presentationURL"), "http://192.168.0.2:5000/");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.deviceType"),
                        "urn:schemas-upnp-org:device:MediaServer:1");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.friendlyName"), "NAS");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturer"), "Synology Inc.");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturerURL"), "http://www.synology.com/");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelDescription"),
                        "Synology DLNA/UPnP Media Server");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelName"), "DS216+II");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelURL"), "http://www.synology.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serialNumber"), "1650O6N558101");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UDN"), "uuid:0011325e-4496-0011-9644-96445e321100");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.presentationURL"), "http://192.168.0.2:5000/");
   }

   BOOST_AUTO_TEST_CASE(NetgearDG834GV4)
   {
      static const auto ssdpAnswer = std::string(
         "<?xml version=\"1.0\"?>\n<root xmlns=\"urn:schemas-upnp-org:device-1-0\"><specVersion><major>1</major><minor>0</minor></specVersion><device><deviceType>urn:schemas-upnp-org:device:InternetGatewayDevice:1</deviceType><friendlyName>NETGEAR DG834GV4 Router</friendlyName><manufacturer>NETGEAR </manufacturer><manufacturerURL>http://www.netgear.com/</manufacturerURL><modelDescription>NETGEAR DG834GV4 Router</modelDescription><modelName>NETGEAR DG834GV4 Router</modelName><modelNumber>5.01.16</modelNumber><modelURL>http://www.netgear.com/</modelURL><serialNumber> </serialNumber><UDN>uuid:824ff22b-8c7d-41c5-a131-44f534e12555</UDN><serviceList><service><serviceType>urn:schemas-upnp-org:service:Layer3Forwarding:1</serviceType><serviceId>urn:upnp-org:serviceId:Layer3Forwarding1</serviceId><controlURL>/ctl/L3F</controlURL><eventSubURL>/evt/L3F</eventSubURL><SCPDURL>/L3F.xml</SCPDURL></service></serviceList><deviceList><device><deviceType>urn:schemas-upnp-org:device:WANDevice:1</deviceType><friendlyName>WANDevice</friendlyName><manufacturer>NETGEAR</manufacturer><manufacturerURL>http://www.netgear.com/</manufacturerURL><modelDescription>WAN Device</modelDescription><modelName>WAN Device</modelName><modelNumber>20070827</modelNumber><modelURL>http://www.netgear.com/</modelURL><serialNumber> </serialNumber><UDN>uuid:824ff22b-8c7d-41c5-a131-44f534e12556</UDN><UPC>DG834GV4</UPC><serviceList><service><serviceType>urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1</serviceType><serviceId>urn:upnp-org:serviceId:WANCommonIFC1</serviceId><controlURL>/ctl/CmnIfCfg</controlURL><eventSubURL>/evt/CmnIfCfg</eventSubURL><SCPDURL>/WANCfg.xml</SCPDURL></service></serviceList><deviceList><device><deviceType>urn:schemas-upnp-org:device:WANConnectionDevice:1</deviceType><friendlyName>WANConnectionDevice</friendlyName><manufacturer>NETGEAR</manufacturer><manufacturerURL>http://www.netgear.com/</manufacturerURL><modelDescription>Residential Gateway</modelDescription><modelName>DG834GV4</modelName><modelNumber>20070827</modelNumber><modelURL>http://www.netgear.com/</modelURL><serialNumber> </serialNumber><UDN>uuid:824ff22b-8c7d-41c5-a131-44f534e12557</UDN><UPC>DG834GV4</UPC><serviceList><service><serviceType>urn:schemas-upnp-org:service:WANIPConnection:1</serviceType><serviceId>urn:upnp-org:serviceId:WANIPConn1</serviceId><controlURL>/ctl/IPConn</controlURL><eventSubURL>/evt/IPConn</eventSubURL><SCPDURL>/WANIPCn.xml</SCPDURL></service></serviceList></device></deviceList></device></deviceList><presentationURL>http://192.168.0.1/</presentationURL></device></root>");

      const shared::http::ssdp::CDiscoveredDevice dd( ssdpAnswer);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.presentationURL"), "http://192.168.0.1/");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.deviceType"),
                        "urn:schemas-upnp-org:device:InternetGatewayDevice:1");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.friendlyName"), "NETGEAR DG834GV4 Router");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturer"), "NETGEAR ");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturerURL"), "http://www.netgear.com/");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelDescription"),                        "NETGEAR DG834GV4 Router");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelName"), "NETGEAR DG834GV4 Router");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelNumber"), "5.01.16");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelURL"), "http://www.netgear.com/");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serialNumber"), " ");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UDN"), "uuid:824ff22b-8c7d-41c5-a131-44f534e12555");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.presentationURL"), "http://192.168.0.1/");
   }

   BOOST_AUTO_TEST_CASE(TpLinkTLWA801N)
   {
      static const auto ssdpAnswer = std::string(
         "<?xml version=\"1.0\"?>\n<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n  <specVersion>\n    <major>1</major>\n    <minor>0</minor>\n  </specVersion>\n  <device>\n    <deviceType>urn:schemas-wifialliance-org:device:WFADevice:1</deviceType>\n    <friendlyName>WPS Access Point</friendlyName>\n    <manufacturer>TP-LINK</manufacturer>\n    <manufacturerURL>http://www.tp-link.com</manufacturerURL>\n    <modelDescription>Wireless N Router</modelDescription>\n    <modelName>TL-WA801N</modelName>\n    <modelNumber>2.0</modelNumber>\n    <modelURL>http://192.168.0.3:80</modelURL>\n    <serialNumber>1.0</serialNumber>\n    <UDN>uuid:00000000-0000-1000-0000-e894f6a317f3</UDN>\n    <UPC>UPC here</UPC>\n    <serviceList>\n      <service>\n        <serviceType>urn:schemas-wifialliance-org:service:WFAWLANConfig:1</serviceType>\n        <serviceId>urn:wifialliance-org:serviceId:WFAWLANConfig1</serviceId>\n        <SCPDURL>wps_scpd.xml</SCPDURL>\n        <controlURL>wps_control</controlURL>\n        <eventSubURL>wps_event</eventSubURL>\n      </service>\n    </serviceList>\n</device>\n</root>\n");

      const shared::http::ssdp::CDiscoveredDevice dd(ssdpAnswer);
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelURL"), "http://192.168.0.3:80");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.deviceType"),
                        "urn:schemas-wifialliance-org:device:WFADevice:1");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.friendlyName"), "WPS Access Point");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturer"), "TP-LINK");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.manufacturerURL"), "http://www.tp-link.com");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelDescription"),                        "Wireless N Router");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelName"), "TL-WA801N");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.modelNumber"), "2.0");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.serialNumber"), "1.0");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UDN"), "uuid:00000000-0000-1000-0000-e894f6a317f3");
      BOOST_CHECK_EQUAL(dd.xmlContent()->get<std::string>("root.device.UPC"), "UPC here");
   }

BOOST_AUTO_TEST_SUITE_END()
