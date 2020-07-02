#include "stdafx.h"
#include "DiscoveredDevice.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         CDiscoveredDevice::CDiscoveredDevice(const std::string& location,
            const std::string& deviceDescription)
            : m_location(location),
              m_ip(ipFromXml(m_location)),
              m_deviceDescription(deviceDescriptionFromXml(deviceDescription))
         {
         }

         std::string CDiscoveredDevice::ip() const
         {
            return m_ip;
         }

         boost::shared_ptr<const CDataContainer> CDiscoveredDevice::deviceDescription() const
         {
            return m_deviceDescription;
         }

         std::string CDiscoveredDevice::ipFromXml(const std::string& deviceDescription)
         {
            const boost::regex reg(R"((\d{1,3}(\.\d{1,3}){3}))");
            boost::smatch match;
            if (!regex_search(deviceDescription, match, reg) || match.length() < 2)
               throw std::runtime_error("Invalid IP found");

            return match[1].str();
         }

         boost::shared_ptr<const CDataContainer> CDiscoveredDevice::deviceDescriptionFromXml(
            const std::string& deviceDescription)
         {
            boost::property_tree::ptree tree;
            std::stringstream inStream;
            inStream << deviceDescription;
            read_xml(inStream, tree);

            // Be careful, JSON writer from Boost is quite limited (bad management of lists :
            // provides something like {"clé":""} instead of {"key":[]}
            //	But is good enough here, as we don't have lists

            std::stringstream outStream;
            write_json(outStream, tree.get_child("root.device"));

            return CDataContainer::make(outStream.str());
         }
      }
   }
}
