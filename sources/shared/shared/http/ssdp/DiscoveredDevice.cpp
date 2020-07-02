#include "stdafx.h"
#include "DiscoveredDevice.h"
#include <boost/property_tree/xml_parser.hpp>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         CDiscoveredDevice::CDiscoveredDevice(const std::string& xmlContent)
            : m_xmlContent(contentFromXml(xmlContent))
         {
         }

         boost::shared_ptr<const boost::property_tree::ptree> CDiscoveredDevice::xmlContent() const
         {
            return m_xmlContent;
         }

         boost::shared_ptr<const boost::property_tree::ptree> CDiscoveredDevice::contentFromXml(
            const std::string& xmlContent)
         {
            if (xmlContent.empty())
               throw std::runtime_error("Invalid XML (empty)");

            auto tree = boost::make_shared<boost::property_tree::ptree>();
            std::stringstream inStream;
            inStream << xmlContent;
            read_xml(inStream, *tree);
            return tree;
         }
      }
   }
}
