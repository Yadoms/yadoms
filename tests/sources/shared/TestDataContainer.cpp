#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"


BOOST_AUTO_TEST_SUITE(TestDataContainer)



enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
   ("EnumValue1", kEnumValue1)
   ("EnumValue2", kEnumValue2)
   ("EnumValue3", kEnumValue3);


BOOST_AUTO_TEST_CASE(SimpleContainer)
{
   shared::CDataContainer dc;

   boost::posix_time::ptime actualDatetime = boost::posix_time::second_clock::universal_time();

   //ensure en new container is empty
   BOOST_CHECK_EQUAL(dc.empty(), true);
   
   dc.set<bool>("BoolParameter", true);
   dc.set<double>("DecimalParameter", 18.4);
   dc.set<EEnumType>("EnumParameter", kEnumValue2);
   dc.set<std::string>("EnumAsStringParameter", "EnumValue1");
   dc.set<int>("IntParameter", 42);
   dc.set<std::string>("Serial port", "tty0");
   dc.set<std::string>("StringParameter", "Yadoms is so powerful !");
   dc.set<int>("MySection.SubIntParameter", 123);
   dc.set<std::string>("MySection.SubStringParameter", "Just a string parameter in the sub-section");
   dc.set<boost::posix_time::ptime>("DateTimeParameter", actualDatetime);



   BOOST_CHECK_EQUAL(dc.get<bool>("BoolParameter"), true);
   BOOST_CHECK_EQUAL(dc.get<double>("DecimalParameter"), 18.4);
   BOOST_CHECK_EQUAL(dc.get<EEnumType>("EnumParameter"), kEnumValue2);
   BOOST_CHECK_EQUAL(dc.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1);
   BOOST_CHECK_EQUAL(dc.get<int>("IntParameter"), 42);
   BOOST_CHECK_EQUAL(dc.get<std::string>("Serial port"), "tty0");
   BOOST_CHECK_EQUAL(dc.get<std::string>("StringParameter"), "Yadoms is so powerful !");
   BOOST_CHECK_EQUAL(dc.get<int>("MySection.SubIntParameter"), 123);
   BOOST_CHECK_EQUAL(dc.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section");
   BOOST_CHECK_EQUAL(dc.get<boost::posix_time::ptime>("DateTimeParameter"), actualDatetime);


   shared::CDataContainer test;
   test.set<int>("int", 5);
   test.set<double>("double", 4.0);
   test.set<std::string>("string", "plouf");

   BOOST_CHECK_EQUAL(test.get<int>("int"), 5);
   BOOST_CHECK_EQUAL(test.get<double>("double"),4.0);
   BOOST_CHECK_EQUAL(test.get<std::string>("string"), "plouf");
   

   shared::CDataContainer subContainer;
   subContainer.set<int>("int4", 6);
   subContainer.set<double>("double1", 8.0);
   subContainer.set<std::string>("string2", "plouf2");

   test.set("config1", subContainer);

   BOOST_CHECK_EQUAL(test.get<double>("config1.double1"), 8.0);
}

BOOST_AUTO_TEST_CASE(CollectionContainer)
{
   shared::CDataContainer test;

   std::vector<int> vi;
   for (int i = 0; i<10; ++i)
      vi.push_back(i);
   test.set < std::vector<int> >("vectorint", vi);
   std::vector<int> vi2 = test.get<std::vector<int>>("vectorint");
   BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end());


   std::vector<double> vd;
   for (int i = 0; i<10; ++i)
      vd.push_back(i * 3.0);
   test.set< std::vector<double> >("vectordouble", vd);
   std::vector<double> vd2 = test.get< std::vector<double> >("vectordouble");
   BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end());
}

BOOST_AUTO_TEST_CASE(Serialization)
{
   const std::string defaultConf("{"
      "\"BoolParameter\": \"true\","
      "\"DecimalParameter\": \"18.4\","
      "\"EnumParameter\": \"12\","
      "\"EnumAsStringParameter\": \"EnumValue1\","
      "\"IntParameter\": \"42\","
      "\"Serial port\": \"tty0\","
      "\"StringParameter\": \"Yadoms is so powerful !\","
      "\"DateTimeParameter\": \"20140702T113500\","
      "\"MySection\": {"
      "\"SubIntParameter\": \"123\","
      "\"SubStringParameter\": \"Just a string parameter in the sub-section\""
      "}"
      "}");

   shared::CDataContainer cfg(defaultConf);

   BOOST_CHECK_EQUAL(cfg.get<bool>("BoolParameter"), true);
   BOOST_CHECK_EQUAL(cfg.get<double>("DecimalParameter"), 18.4);
   BOOST_CHECK_EQUAL(cfg.get<int>("IntParameter"), 42);
   BOOST_CHECK_EQUAL(cfg.get<EEnumType>("EnumParameter"), kEnumValue2);
   BOOST_CHECK_EQUAL(cfg.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1);
   BOOST_CHECK_EQUAL(cfg.get<std::string>("Serial port"), "tty0");
   BOOST_CHECK_EQUAL(cfg.get<std::string>("StringParameter"), "Yadoms is so powerful !");
   BOOST_CHECK_EQUAL(cfg.get<int>("MySection.SubIntParameter"), 123);
   BOOST_CHECK_EQUAL(cfg.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section");

   boost::posix_time::ptime expected(boost::gregorian::date(2014, 7, 2), boost::posix_time::hours(11) + boost::posix_time::minutes(35) + boost::posix_time::seconds(0));
   BOOST_CHECK_EQUAL(cfg.get<boost::posix_time::ptime>("DateTimeParameter"), expected);

   //check that serialization match original values
   //just remove space, \n, \t and \r from strings
   std::string str = cfg.serialize();
   std::string str2 = defaultConf;

   str.erase(std::remove_if(str.begin(), str.end(), boost::is_any_of(" \t\n\r")), str.end());
   str2.erase(std::remove_if(str2.begin(), str2.end(), boost::is_any_of(" \t\n\r")), str2.end());

   BOOST_CHECK_EQUAL(str, str2);
}


class CTestClass : public shared::IDataContainable
{
public:
   CTestClass()
      :m_aIntValue(0), m_dValue(0), m_sValue("")
   {

   }
   CTestClass(int i, double d, std::string s)
      :m_aIntValue(i), m_dValue(d), m_sValue(s)
   {

   }

   virtual void extractContent(shared::CDataContainer & cont) const
   {
      cont.set("Value1", m_aIntValue);
      cont.set("Value2", m_dValue);
      cont.set("Value3", m_sValue);
   }

   virtual void fillFromContent(const shared::CDataContainer & initialData)
   {
      m_aIntValue = initialData.get<int>("Value1");
      m_dValue = initialData.get<double>("Value2");
      m_sValue = initialData.get<std::string>("Value3");
   }

   int m_aIntValue;
   double m_dValue;
   std::string m_sValue;
};

BOOST_AUTO_TEST_CASE(DataContainable)
{
   CTestClass obj(1, 42.0, "test de datacontainble");
   shared::CDataContainer cont;
   cont.setObject("myobject", &obj);

   CTestClass result;
   cont.getObject("myobject", &result);

   BOOST_CHECK_EQUAL(obj.m_aIntValue, result.m_aIntValue);
}

BOOST_AUTO_TEST_SUITE_END()