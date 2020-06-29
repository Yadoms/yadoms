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
         CDiscoveredDevice::CDiscoveredDevice(const std::string& deviceDescription)
            : m_description(fromXml(deviceDescription))
         {
         }

         std::string CDiscoveredDevice::ip() const
         {
            const auto& fullUrl = m_description->get<std::string>("root.URLBase");

            const boost::regex reg(R"((\d{1,3}(\.\d{1,3}){3}))");
            boost::smatch match;
            if (!regex_search(fullUrl, match, reg) || match.length() < 2)
               throw std::runtime_error("Invalid IP found");

            return match[1].str();
         }

         std::string CDiscoveredDevice::findDeviceTag(const std::string& tagName) const
         {
            const auto deviceBloc = m_description->getChild("device");
            return deviceBloc->get<std::string>(tagName);
         }

         boost::shared_ptr<CDataContainer> CDiscoveredDevice::fromXml(const std::string& deviceDescription)
         {
            boost::property_tree::ptree tree;
            read_xml(deviceDescription, tree);

            // Be careful, JSON writer from Boost is quite limited (bad management of lists :
            // provides something like {"clé":""} instead of {"key":[]}
            //	But is good enough here, as we don't have lists

            std::stringstream ss;
            write_json(ss, tree);

            return CDataContainer::make(ss.str());
         }
      }
   }
}
