#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/http/HttpMethods.h"

class CHttpMethodsPrivateAccess : public shared::CHttpMethods
{
public:
   static shared::CDataContainer callProcessXmlNode(const boost::property_tree::ptree& node)
   {
      return processXmlNode(node);
   }
};

BOOST_AUTO_TEST_SUITE(TestHttpMethods)

   BOOST_AUTO_TEST_CASE(InternalProcessXmlNode)
   {
      std::istringstream input(std::string(
         "<?xml version=\"1.0\"?> \
          <root rootAttribute=\"rootAttributeValue\"> \
            <firstNode>2</firstNode> \
            <secondNode stringAttribute=\"attributeValue\" intAttribute=\"3\"> \
              <item1>value1</item1> \
              <item1>repeatedValue1</item1> \
              <item2>1234</item2> \
            </secondNode> \
            <thirdNode otherAttribute=\"other\"> \
            </thirdNode> \
            <emptyItem/> \
          </root>"));

      boost::property_tree::ptree tree;
      read_xml(input, tree);
      const auto output = CHttpMethodsPrivateAccess::callProcessXmlNode(tree);

      output.printToLog(std::cout);//TODO virer

      BOOST_CHECK_EQUAL(output.get<int>("firstNode"), 2);
      BOOST_CHECK_EQUAL(output.get<std::string>("secondNode.stringAttribute"), "attributeValue");
      BOOST_CHECK_EQUAL(output.get<int>("secondNode.intAttribute"), 3);
      BOOST_CHECK_EQUAL(output.get<std::string>("secondNode.item1"), "value1");
      BOOST_CHECK_EQUAL(output.get<int>("secondNode.item2"), 1234);

      //TODO
   }

BOOST_AUTO_TEST_SUITE_END()
