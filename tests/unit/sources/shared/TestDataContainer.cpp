#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"
#include "../../../../sources/server/web/rest/Result.h"
#include "../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestDataContainer)


enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3 = 14,
   kEnumValue4,
   kEnumValue5,
   kEnumValue6,
   kEnumValue7,
};

static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
("EnumValue1", kEnumValue1)
("EnumValue2", kEnumValue2)
("EnumValue3", kEnumValue3);


BOOST_AUTO_TEST_CASE(DataContainerEmptyContainer)
{
   BOOST_CHECK_NO_THROW(shared::CDataContainer());
   BOOST_CHECK_EQUAL(shared::CDataContainer().empty(), true);
   BOOST_CHECK_EQUAL(shared::CDataContainer("{}").empty(), true);
   BOOST_CHECK_THROW(shared::CDataContainer(""), std::exception);
}

BOOST_AUTO_TEST_CASE(SimpleContainer)
{
   shared::CDataContainer dc;

   auto actualDatetime = boost::posix_time::second_clock::universal_time();

   //ensure en new container is empty
   BOOST_CHECK_EQUAL(dc.empty(), true);

   //insert all simple data
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

   //check data are correctly retreived
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

   //another test for a sub container
   shared::CDataContainer test;
   test.set<int>("int", 5);
   test.set<double>("double", 4.0);
   test.set<std::string>("string", "plouf");

   BOOST_CHECK_EQUAL(test.get<int>("int"), 5);
   BOOST_CHECK_EQUAL(test.get<double>("double"), 4.0);
   BOOST_CHECK_EQUAL(test.get<std::string>("string"), "plouf");

   shared::CDataContainer subContainer;
   subContainer.set<int>("int4", 6);
   subContainer.set<double>("double1", 8.0);
   subContainer.set<std::string>("string2", "plouf2");

   test.set("config1", subContainer);

   BOOST_CHECK_EQUAL(test.get<double>("config1.double1"), 8.0);


   //check for shared_ptr
   auto shs(boost::make_shared<std::string>("un shared ptr"));
   dc.set("StringSharedPtr", shs);

   auto shs2 = dc.get<boost::shared_ptr<std::string>>("StringSharedPtr");
   BOOST_CHECK_EQUAL(*(shs.get()) == *(shs2.get()), true);
}

BOOST_AUTO_TEST_CASE(JsonOldformatStyle)
{
   const std::string oldJsonFormatStyle("{"
      "\"BoolParameter\": \"true\","
      "\"BoolParameter2\": \"false\","
      "\"DecimalParameter\": \"18.4\","
      "\"EnumParameter\": \"12\","
      "\"EnumAsStringParameter\": \"EnumValue1\","
      "\"IntParameter\": \"42\""
      "}");

   shared::CDataContainer cfg(oldJsonFormatStyle);

   BOOST_CHECK_EQUAL(cfg.get<bool>("BoolParameter"), true);
   BOOST_CHECK_EQUAL(cfg.get<bool>("BoolParameter2"), false);
   BOOST_CHECK_EQUAL(cfg.get<double>("DecimalParameter"), 18.4);
   BOOST_CHECK_EQUAL(cfg.get<int>("IntParameter"), 42);
   BOOST_CHECK_EQUAL(cfg.get<EEnumType>("EnumParameter"), kEnumValue2);
   BOOST_CHECK_EQUAL(cfg.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1);
}

BOOST_AUTO_TEST_CASE(CollectionContainer)
{
   shared::CDataContainer test;

   //check vector of int
   std::vector<int> vi;
   for (auto i = 0; i < 10; ++i)
      vi.push_back(i);
   test.set<std::vector<int>>("vectorint", vi);
   auto vi2 = test.get<std::vector<int>>("vectorint");
   BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end());

   //check vector of double
   std::vector<double> vd;
   for (auto i = 0; i < 10; ++i)
      vd.push_back(i * 3.0);
   test.set<std::vector<double>>("vectordouble", vd);
   auto vd2 = test.get<std::vector<double>>("vectordouble");
   BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end());

   //check vector of EEnumType
   std::vector<EEnumType> ve;
   ve.push_back(kEnumValue2);
   ve.push_back(kEnumValue4);
   ve.push_back(kEnumValue5);
   ve.push_back(kEnumValue7);
   test.set("vectorenum", ve);
   std::vector<EEnumType> ve2 = test.get<std::vector<EEnumType>>("vectorenum");
   BOOST_CHECK_EQUAL_COLLECTIONS(ve.begin(), ve.end(), ve2.begin(), ve2.end());

   //check vector of shared_ptr<int>
   std::vector<boost::shared_ptr<int>> vish;
   for (auto i = 0; i < 10; ++i)
      vish.push_back(boost::make_shared<int>(i));
   test.set("vectorintsh", vish);
   auto vish2 = test.get<std::vector<boost::shared_ptr<int>>>("vectorintsh");
   auto vish2bis = test.get<std::vector<int>>("vectorintsh");
   BOOST_CHECK_EQUAL(vish.size(), vish2.size());
   for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == *(vish2[i].get()), true);

   BOOST_CHECK_EQUAL(vish.size(), vish2bis.size());
   for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == vish2bis[i], true);

   //check vector of shared_ptr<double>
   std::vector<boost::shared_ptr<double>> vdsh;
   for (unsigned int i = 0; i < 10; ++i)
      vdsh.push_back(boost::make_shared<double>(i * 42.0));
   test.set("vectordoublesh", vdsh);
   auto vdsh2 = test.get<std::vector<boost::shared_ptr<double>>>("vectordoublesh");
   auto vdsh2bis = test.get<std::vector<double>>("vectordoublesh");
   BOOST_CHECK_EQUAL(vdsh.size(), vdsh2.size());
   for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == *(vdsh2[i].get()), true);

   BOOST_CHECK_EQUAL(vdsh.size(), vdsh2bis.size());
   for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == vdsh2bis[i], true);

   //check vector of std::string
   std::vector<std::string> vstr;
   for (unsigned int i = 0; i < 10; ++i)
      vstr.push_back((boost::format("string %1%") % i).str());
   test.set("vectorstring", vstr);

   auto vstr2 = test.get<std::vector<std::string>>("vectorstring");
   BOOST_CHECK_EQUAL(vstr.size(), vstr2.size());
   for (unsigned int i = 0; i < vstr2.size(); ++i)
      BOOST_CHECK_EQUAL(vstr[i] == vstr2[i], true);

   //check vector of CDataContainer

   shared::CDataContainer cond1;
   cond1.set("is.keyword", 8);
   cond1.set("is.expectedValue", "32");

   shared::CDataContainer cond2;
   cond2.set("is.keyword", 9);
   cond2.set("is.expectedValue", 34);

   shared::CDataContainer cond3;
   cond3.set("is.keyword", 10);
   cond3.set("is.expectedValue", ve);

   shared::CDataContainer conditions;

   std::vector<shared::CDataContainer> allconditions;
   allconditions.push_back(cond1);
   allconditions.push_back(cond2);
   allconditions.push_back(cond3);

   conditions.set("and", allconditions);

   //do checks
   auto getAllCond = conditions.get<std::vector<shared::CDataContainer>>("and");

   BOOST_CHECK_EQUAL(allconditions.size(), getAllCond.size());

   auto getCond1 = getAllCond[0];
   auto getCond2 = getAllCond[1];
   auto getCond3 = getAllCond[2];

   BOOST_CHECK_EQUAL(cond1, getCond1);
   BOOST_CHECK_EQUAL(cond2, getCond2);
   BOOST_CHECK_EQUAL(cond3, getCond3);

   BOOST_CHECK_EQUAL_COLLECTIONS(allconditions.begin(), allconditions.end(), getAllCond.begin(), getAllCond.end());
}

BOOST_AUTO_TEST_CASE(ContainerToVectorOfContainers)
{
   const shared::CDataContainer inIsNotAnArray("\
      {\
         \"changelogUrl\": \"http://www.yadoms.com/downloads/update/widgets/moon/changelog.md\",\
            \"0\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0\"\
         },\
         \"1\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.2\"\
         },\
         \"2\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0-rc.1\"\
         },\
         \"3\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.1\"\
         }\
      }");
   BOOST_CHECK_THROW(const auto out = inIsNotAnArray.get<std::vector<shared::CDataContainer>>(), shared::exception::COutOfRange);
}


boost::shared_ptr<shared::serialization::IDataSerializable> maketest(const unsigned int testcount)
{
   shared::CDataContainer result;
   std::vector<std::string> pluginCollection;

   for (unsigned int i = 0; i < testcount; ++i)
      pluginCollection.push_back((boost::format("plugin %1%") % i).str());

   result.set("plugins", pluginCollection);

   return web::rest::CResult::GenerateSuccess(result);
}


BOOST_AUTO_TEST_CASE(RapidJsonValueSlash)
{
   //this test check that keys and values containing slash are correctly handled
   const std::string key = "French/France";
   const std::string value = "Euros/Dollars";

   shared::CDataContainer dc;
   dc.set(key, value);

   BOOST_CHECK_EQUAL(dc.exists("French"), false);
   BOOST_CHECK_EQUAL(dc.exists(key), true);
   BOOST_CHECK_EQUAL(dc.get<std::string>(key), value);
}


std::string str1("str1");
std::string str2("str2");

std::string & getString1() { return str1; }
const std::string & getString2() { return str2; }

shared::CDataContainer globalContainer;
shared::CDataContainer& getGlobalContainer() { return globalContainer; }

shared::CDataContainer getDeviceInfo()
{
   shared::CDataContainer d;

   {
      std::string id = "id";
      std::string sNodeName = "sNodeName";
      std::string sNodeManufacturer = "sNodeManufacturer";
      std::string sNodeManufacturerId = "sNodeManufacturerId";
      std::string sNodeProductName = "sNodeProductName";
      std::string sNodeProductId = "sNodeProductId";
      std::string sNodeProductType = "sNodeProductType";
      std::string sNodeLocation = "sNodeLocation";
      std::string sNodeType = "sNodeType";
      std::string sNodeVersion = "sNodeVersion";
      std::string sNodePlusType = "sNodePlusType";
      std::string sNodeDeviceTypeString = "sNodeDeviceTypeString";
      std::string sNodeRole = "sNodeRole";

      d.set("name", id);
      d.set("friendlyName", sNodeName);

      shared::CDataContainer details;
      details.set("Manufacturer", sNodeManufacturer);

      details.set("ManufacturerId", sNodeManufacturerId);
      details.set("Product", sNodeProductName);
      details.set("ProductId", sNodeProductId);
      details.set("ProductType", sNodeProductType);
      details.set("Location", sNodeLocation);
      details.set("Type", sNodeType);
      details.set("Version", sNodeVersion);
      details.set("ZWave+Type", sNodePlusType);
      details.set("DeviceType", sNodeDeviceTypeString);
      details.set("Role", sNodeRole);

      details.set("IsRouting", true);
      details.set("IsListening", false);
      details.set("IsFLiRS", true);
      details.set("IsBeaming", false);
      details.set("IsAwake", true);
      details.set("IsFailed", false);
      details.set("IsSecurity", true);
      details.set("IsZWave+", false);

      details.set("str1", getString1());
      details.set("str2", getString2());

      d.set("details", details);
   }

   return d;
}


BOOST_AUTO_TEST_CASE(RapidJsonStringMemory)
{
   shared::CDataContainer a;
   shared::CDataContainer b;

   //basic cases
   {
      std::string string4("str4");
      std::string string5("str5");

      a.set("string1", getString1());
      a.set("string2", getString2());
      a.set("string3", "str3");
      a.set("string4", string4);
      a.set("string5", string5.c_str());

      BOOST_CHECK_EQUAL(a.get<std::string>("string1"), "str1");
      BOOST_CHECK_EQUAL(a.get<std::string>("string2"), "str2");
      BOOST_CHECK_EQUAL(a.get<std::string>("string3"), "str3");
      BOOST_CHECK_EQUAL(a.get<std::string>("string4"), "str4");
      BOOST_CHECK_EQUAL(a.get<std::string>("string5"), "str5");
   }

   //check out of {}
   BOOST_CHECK_EQUAL(a.get<std::string>("string1"), "str1");
   BOOST_CHECK_EQUAL(a.get<std::string>("string2"), "str2");
   BOOST_CHECK_EQUAL(a.get<std::string>("string3"), "str3");
   BOOST_CHECK_EQUAL(a.get<std::string>("string4"), "str4");
   BOOST_CHECK_EQUAL(a.get<std::string>("string5"), "str5");

   //check merge operations
   {
      b.mergeFrom(a);
      BOOST_CHECK_EQUAL(b.get<std::string>("string1"), "str1");
      BOOST_CHECK_EQUAL(b.get<std::string>("string2"), "str2");
      BOOST_CHECK_EQUAL(b.get<std::string>("string3"), "str3");
      BOOST_CHECK_EQUAL(b.get<std::string>("string4"), "str4");
      BOOST_CHECK_EQUAL(b.get<std::string>("string5"), "str5");
   }
   BOOST_CHECK_EQUAL(b.get<std::string>("string1"), "str1");
   BOOST_CHECK_EQUAL(b.get<std::string>("string2"), "str2");
   BOOST_CHECK_EQUAL(b.get<std::string>("string3"), "str3");
   BOOST_CHECK_EQUAL(b.get<std::string>("string4"), "str4");
   BOOST_CHECK_EQUAL(b.get<std::string>("string5"), "str5");


   {
      shared::CDataContainer info = getDeviceInfo();

      BOOST_CHECK_EQUAL(info.get<std::string>("details.str1"), "str1");
      BOOST_CHECK_EQUAL(info.get<std::string>("details.str2"), "str2");

      shared::CDataContainer plop;

      plop.mergeFrom(info);
      a.mergeFrom(plop);

      getGlobalContainer().mergeFrom(plop);
   }


   //check after global objects modifications
   str1 = "anotherStr1";
   str2 = "anotherStr2";

   BOOST_CHECK_EQUAL(a.get<std::string>("string1"), "str1");
   BOOST_CHECK_EQUAL(a.get<std::string>("string2"), "str2");
   BOOST_CHECK_EQUAL(a.get<std::string>("string3"), "str3");
   BOOST_CHECK_EQUAL(a.get<std::string>("string4"), "str4");
   BOOST_CHECK_EQUAL(a.get<std::string>("string5"), "str5");

   BOOST_CHECK_EQUAL(b.get<std::string>("string1"), "str1");
   BOOST_CHECK_EQUAL(b.get<std::string>("string2"), "str2");
   BOOST_CHECK_EQUAL(b.get<std::string>("string3"), "str3");
   BOOST_CHECK_EQUAL(b.get<std::string>("string4"), "str4");
   BOOST_CHECK_EQUAL(b.get<std::string>("string5"), "str5");

   BOOST_CHECK_EQUAL(a.get<std::string>("details.str1"), "str1");
   BOOST_CHECK_EQUAL(a.get<std::string>("details.str2"), "str2");

   BOOST_CHECK_EQUAL(getGlobalContainer().get<std::string>("details.str1"), "str1");
   BOOST_CHECK_EQUAL(getGlobalContainer().get<std::string>("details.str2"), "str2");
}

BOOST_AUTO_TEST_CASE(RapidJsonInitAndCopy)
{
   /*
   This test case is an example from Zwave plugin which illustrate a bad object copy.
   */
   typedef std::pair<shared::CDataContainer, int> DeviceInfoAndState;
   typedef std::map<std::string, DeviceInfoAndState > DeviceCache;
   DeviceCache m_deviceCache;

   {
      shared::CDataContainer info = getDeviceInfo();
      std::string l("test3");

      //directly access with []
      DeviceInfoAndState &c = m_deviceCache["mydevice2"];
      c.first.mergeFrom(info);

      m_deviceCache["mydevice2"].first.set("test1", "test2");
      m_deviceCache["mydevice2"].first.set("test2", l);
      
      //insert pair instead of []
      shared::CDataContainer a("{}");
      m_deviceCache.insert(std::make_pair("mydevice", std::make_pair(a, 42)));

      DeviceInfoAndState & b = m_deviceCache["mydevice"];
      b.first.mergeFrom(info);
   }

   {
      BOOST_CHECK_EQUAL(m_deviceCache["mydevice2"].first.get("test1"), "test2");

      auto &d = m_deviceCache["mydevice2"];
      auto &e = m_deviceCache["mydevice2"].first;

      BOOST_CHECK_EQUAL(e.exists("test2"), true);
      BOOST_CHECK_EQUAL(e.get("test2"), "test3");

      BOOST_CHECK_EQUAL(d.first.exists("test2"), true);
      BOOST_CHECK_EQUAL(d.first.get("test2"), "test3");

      BOOST_CHECK_EQUAL(m_deviceCache["mydevice2"].first.exists("test2"), true);
      BOOST_CHECK_EQUAL(m_deviceCache["mydevice2"].first.get("test2"), "test3");
   }
}

BOOST_AUTO_TEST_CASE(DataCopy)
{
   shared::CDataContainer dc;
   const unsigned int testcount = 10;

   //ensure braces are used => in that case, inner container will be deleted to brace close
   {
      //check vector of std::string
      shared::CDataContainer test;
      std::vector<std::string> vstr;
      for (unsigned int i = 0; i < testcount; ++i)
         vstr.push_back((boost::format("string %1%") % i).str());
      test.set("vectorstring", vstr);
      dc = test;
   }

   auto vstr2 = dc.get<std::vector<std::string>>("vectorstring");
   BOOST_CHECK_EQUAL(vstr2.size(), testcount);
   for (unsigned int i = 0; i < vstr2.size(); ++i)
   {
      std::string loc = (boost::format("string %1%") % i).str();
      BOOST_CHECK_EQUAL(vstr2[i] == loc, true);
   }

   //the following test illustrate a bad string allocation (normally datacontainer copy should keep allocation; 
   //but if a string is copied into rapidjson value without allocator, then the string is kept as a simple reference
   //and this test fails if string are destroyed
   auto k = maketest(testcount);
   shared::CDataContainer dc2(k->serialize());
   auto vstr = dc2.get<std::vector<std::string>>("data.plugins");
   BOOST_CHECK_EQUAL(vstr.size(), testcount);
   for (unsigned int i = 0; i < vstr.size(); ++i)
   {
      std::string loc = (boost::format("plugin %1%") % i).str();
      BOOST_CHECK_EQUAL(vstr[i] == loc, true);
   }
}


BOOST_AUTO_TEST_CASE(Serialization)
{
   const std::string defaultConf("{"
      "\"BoolParameter\": true,"
      "\"DecimalParameter\": 18.4,"
      "\"EnumParameter\": 12,"
      "\"EnumAsStringParameter\": \"EnumValue1\","
      "\"IntParameter\": 42,"
      "\"Serial port\": \"tty0\","
      "\"StringParameter\": \"Yadoms is so powerful !\","
      "\"DateTimeParameter\": \"20140702T113500\","
      "\"MySection\": {"
      "\"SubIntParameter\": 123,"
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

   boost::posix_time::ptime expected(boost::gregorian::date(2014, 7, 2),
      boost::posix_time::hours(11) + boost::posix_time::minutes(35) + boost::posix_time::seconds(0));
   BOOST_CHECK_EQUAL(cfg.get<boost::posix_time::ptime>("DateTimeParameter"), expected);

   //check that serialization match original values
   //just remove space, \n, \t and \r from strings
   auto str = cfg.serialize();
   auto str_2 = defaultConf;

   str.erase(std::remove_if(str.begin(), str.end(), boost::is_any_of(" \t\n\r")), str.end());
   str_2.erase(std::remove_if(str_2.begin(), str_2.end(), boost::is_any_of(" \t\n\r")), str_2.end());

   BOOST_CHECK_EQUAL(str, str_2);


   std::stringstream ss;
   ss << cfg;

   shared::CDataContainer cfgOut;
   ss >> cfgOut;

   BOOST_CHECK_EQUAL(cfg.serialize(), cfgOut.serialize());

   //YADOMS_LOG(information) << cfg;
}


BOOST_AUTO_TEST_CASE(CheckExistance)
{
   const std::string defaultConf("{"
      "\"BoolParameter\": \"true\","
      "\"DecimalParameter\": \"18.4\","
      "\"EnumParameter\": \"12\","
      "\"ArrayParameter\": [1,2,3,4],"
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

   //check path existance
   BOOST_CHECK_EQUAL(cfg.exists(""), true);
   BOOST_CHECK_EQUAL(cfg.exists("BoolParameter"), true);
   BOOST_CHECK_EQUAL(cfg.exists("MySection"), true);
   BOOST_CHECK_EQUAL(cfg.exists("MySection.SubIntParameter"), true);

   BOOST_CHECK_EQUAL(cfg.exists("SubIntParameter"), false);
   BOOST_CHECK_EQUAL(cfg.exists("MySection2"), false);
   BOOST_CHECK_EQUAL(cfg.exists("String Parameter"), false);

   //check child existance
   BOOST_CHECK_EQUAL(cfg.containsChild(""), true);
   BOOST_CHECK_EQUAL(cfg.containsChild("MySection"), true);
   BOOST_CHECK_EQUAL(cfg.containsChild("ArrayParameter"), false);
   BOOST_CHECK_EQUAL(cfg.containsChild("BoolParameter"), false);
   BOOST_CHECK_EQUAL(cfg.containsChild("MySection.SubIntParameter"), false);

   BOOST_CHECK_EQUAL(cfg.containsChildArray("ArrayParameter"), true);
   BOOST_CHECK_EQUAL(cfg.containsChildArray("MySection"), false);

   //check value existance
   BOOST_CHECK_EQUAL(cfg.containsValue(""), false);
   BOOST_CHECK_EQUAL(cfg.containsValue("MySection"), false);
   BOOST_CHECK_EQUAL(cfg.containsValue("BoolParameter"), true);
   BOOST_CHECK_EQUAL(cfg.containsValue("MySection.SubIntParameter"), true);
}

BOOST_AUTO_TEST_CASE(CurrentNodeTests)
{
   const std::string testPlatformContent("{"
      "\"supportedPlatforms\": {"
      "\"mac\" : \"none\","
      "\"windows\" : { \"from\":\"3.1\" },"
      "\"raspberry\" : \"all\","
      "\"others\" : \"supported\""
      "}, "
      "\"supportedPlatforms2\":\"all\""
      "}");

   shared::CDataContainer testPf(testPlatformContent);

   //subnode test
   BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms"), true);
   auto supportedPf = testPf.get<shared::CDataContainer>("supportedPlatforms");
   BOOST_CHECK_EQUAL(supportedPf.containsChild(), true);
   BOOST_CHECK_EQUAL(supportedPf.containsValue(), false);
   BOOST_CHECK_EQUAL(supportedPf.get<std::string>("mac"), "none");
   BOOST_CHECK_EQUAL(supportedPf.get<std::string>("raspberry"), "all");

   //value test
   BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms2"), true);
   auto supportedPf2 = testPf.get<shared::CDataContainer>("supportedPlatforms2");
   BOOST_CHECK_EQUAL(supportedPf2.containsChild(), false);
   BOOST_CHECK_EQUAL(supportedPf2.containsValue(), true);
   BOOST_CHECK_EQUAL(supportedPf2.get<std::string>(), "all");
}

class CTestClass : public shared::IDataContainable
{
public:
   CTestClass()
      : m_aIntValue(0), m_dValue(0), m_sValue("")
   {
   }

   CTestClass(int i, double d, std::string s)
      : m_aIntValue(i), m_dValue(d), m_sValue(s)
   {
   }

   void extractContent(shared::CDataContainer& cont) const override
   {
      cont.set("Value1", m_aIntValue);
      cont.set("Value2", m_dValue);
      cont.set("Value3", m_sValue);
   }

   void fillFromContent(const shared::CDataContainer& initialData) override
   {
      m_aIntValue = initialData.get<int>("Value1");
      m_dValue = initialData.get<double>("Value2");
      m_sValue = initialData.get<std::string>("Value3");
   }

   void fillFromSerializedString(const std::string& serializedData) override
   {
      const shared::CDataContainer deserializeData(serializedData);
      fillFromContent(deserializeData);
   }

   bool equals(const CTestClass& rhs) const
   {
      return m_aIntValue == rhs.m_aIntValue && m_dValue == rhs.m_dValue && m_sValue == rhs.m_sValue;
   }

   bool operator!=(const CTestClass& rhs) const
   {
      return !equals(rhs);
   }

   friend std::ostream& operator <<(std::ostream& ostr, CTestClass const& str)
   {
      return ostr;
   }

private:
   int m_aIntValue;
   double m_dValue;
   std::string m_sValue;
};

BOOST_AUTO_TEST_CASE(DataContainable)
{
   //containeur simple de IDataContainable
   CTestClass obj(1, 42.0, "test of datacontainable");
   shared::CDataContainer cont;
   cont.set("myobject", obj);
   auto result = cont.get<CTestClass>("myobject");
   BOOST_CHECK_EQUAL(obj.equals(result), true);

   //containeur de boost::shared_ptr<IDataContainable>
   auto sp(boost::make_shared<CTestClass>(2, 43.0, "string1"));
   shared::CDataContainer cont2;
   cont2.set("myobject", sp);
   auto result2 = cont2.get<boost::shared_ptr<CTestClass>>("myobject");
   auto result2bis = cont2.get<CTestClass>("myobject");
   BOOST_CHECK_EQUAL(result2->equals(*sp.get()), true);
   BOOST_CHECK_EQUAL(result2bis.equals(*sp.get()), true);

   //containeur simple de std::vector<IDataContainable>
   std::vector<CTestClass> vc;
   for (auto i = 0; i < 10; ++i)
      vc.push_back(CTestClass(i, 42.0 * i, "test of std::vector<IDataContainable>"));
   shared::CDataContainer contvec;
   contvec.set("mycollection", vc);
   auto vc2 = contvec.get<std::vector<CTestClass>>("mycollection");
   BOOST_CHECK_EQUAL_COLLECTIONS(vc.begin(), vc.end(), vc2.begin(), vc2.end());

   //containeur simple de std::vector< boost::shared_ptr<IDataContainable> >
   std::vector<boost::shared_ptr<CTestClass>> vcsh;
   for (auto i = 0; i < 10; ++i)
      vcsh.push_back(boost::make_shared<CTestClass>(i, 42.0 * i, "test of std::vector<IDataContainable>"));
   shared::CDataContainer contvecsh;
   contvecsh.set("mycollectionofshared", vcsh);
   auto vcsh2 = contvecsh.get<std::vector<boost::shared_ptr<CTestClass>>>("mycollectionofshared");
   auto vc2bis = contvecsh.get<std::vector<CTestClass>>("mycollectionofshared");
   BOOST_CHECK_EQUAL(vcsh.size(), vcsh2.size());
   for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(*vcsh2[i].get()), true);

   BOOST_CHECK_EQUAL(vcsh.size(), vc2bis.size());
   for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(vc2bis[i]), true);
}


BOOST_AUTO_TEST_CASE(Field)
{
   shared::CField<int> fi(10);
   shared::CField<double> fd(12.3);
   shared::CField<std::string> fs("this is a test");
   shared::CField<EEnumType> fe(kEnumValue2);
   shared::CField<CTestClass> fdc(CTestClass(5, 42.0, "test of datacontainble"));


   shared::CDataContainer dc;

   dc.set("FieldInt", fi);
   dc.set("FieldDouble", fd);
   dc.set("FieldString", fs);
   dc.set("FieldEnum", fe);
   dc.set("FieldDataContainable", fdc);

   //check data are correctly retreived
   BOOST_CHECK_EQUAL(dc.get<int>("FieldInt"), fi());
   BOOST_CHECK_EQUAL(dc.get<double>("FieldDouble"), fd());
   BOOST_CHECK_EQUAL(dc.get<std::string>("FieldString"), fs());
   BOOST_CHECK_EQUAL(dc.get<EEnumType>("FieldEnum"), fe());
   BOOST_CHECK_EQUAL(dc.get<CTestClass>("FieldDataContainable").equals(fdc()), true);


   //vector de field
   std::vector<shared::CField<int>> vfi;
   for (auto i = 0; i < 10; ++i)
      vfi.push_back(shared::CField<int>(i));

   dc.set("VectorFieldInt", vfi);
   auto vi2 = dc.get<std::vector<int>>("VectorFieldInt");
   BOOST_CHECK_EQUAL_COLLECTIONS(vfi.begin(), vfi.end(), vi2.begin(), vi2.end());
}


BOOST_AUTO_TEST_CASE(Path)
{
   shared::CField<int> fi(10);

   //standard path separator using '.'
   shared::CDataContainer dc;
   dc.set("secA.secB.valC", fi);
   BOOST_CHECK_EQUAL(dc.get<int>("secA.secB.valC"), fi());
   BOOST_CHECK_EQUAL(dc.get<shared::CDataContainer>("secA").get<shared::CDataContainer>("secB").get<int>("valC"), fi());

   //no path using separator 0x00
   dc.set("secD.secE.valC", fi, 0x00);
   BOOST_CHECK_EQUAL(dc.get<int>("secD.secE.valC", 0x00), fi());
   BOOST_CHECK_EQUAL(dc.exists("secD.secE.valC"), false);
   BOOST_CHECK_THROW(dc.get<int>("secD.secE.valC"), std::exception);
   BOOST_CHECK_THROW(dc.get<shared::CDataContainer>("secD"), std::exception);
}


BOOST_AUTO_TEST_CASE(SimpleConstruction)
{
   shared::CDataContainer dc("1");
   BOOST_CHECK_EQUAL(dc.serialize(), "1");
}


BOOST_AUTO_TEST_CASE(Merge)
{
   shared::CDataContainer to(
      "{"
      "   \"developerMode\": \"false\","
      "   \"location\":"
      "   {"
      "      \"latitude\": \"48.853\","
      "      \"longitude\": \"2.35\","
      "      \"timezone\": \"Europe/Paris\""
      "   },"
      "   \"language\": \"en\","
      "   \"advancedParameters\": \"false\","
      "   \"dateFormatString\": \"LLL\","
      "   \"refreshPage\": \"false\","
      "   \"basicAuthentication\":"
      "   {"
      "      \"active\": \"false\","
      "      \"user\": \"admin\","
      "      \"password\": \"\""
      "   },"
      "   \"multilevelNorMerged\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"3\""
      "         }"
      "      }"
      "   },"
      "   \"multilevelMerged\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"3\""
      "         }"
      "      }"
      "   }"
      "}");

   shared::CDataContainer from(
      "{"
      "   \"developerMode\": \"true\","
      "   \"location\":"
      "   {"
      "      \"latitude\": \"52.5\""
      "   },"
      "   \"language\": \"fr\","
      "   \"advancedParameters\": \"true\","
      "   \"refreshPage\": \"false\","
      "   \"basicAuthentication\":"
      "   {"
      "      \"active\": \"true\","
      "      \"user\": \"admin\","
      "      \"password\": \"1234\""
      "   },"
      "   \"multilevelMerged\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   },"
      "   \"newBloc\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   }"
      "}");

   const shared::CDataContainer expected(
      "{"
      "   \"developerMode\": \"true\","
      "   \"location\":"
      "   {"
      "      \"latitude\": \"52.5\","
      "      \"longitude\": \"2.35\","
      "      \"timezone\": \"Europe/Paris\""
      "   },"
      "   \"language\": \"fr\","
      "   \"advancedParameters\": \"true\","
      "   \"dateFormatString\": \"LLL\","
      "   \"refreshPage\": \"false\","
      "   \"basicAuthentication\":"
      "   {"
      "      \"active\": \"true\","
      "      \"user\": \"admin\","
      "      \"password\": \"1234\""
      "   },"
      "   \"multilevelNorMerged\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"3\""
      "         }"
      "      }"
      "   },"
      "   \"multilevelMerged\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   },"
      "   \"newBloc\":"
      "   {"
      "      \"valueLevel1\": \"1\","
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": \"2\","
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   }"
      "}");


   to.mergeFrom(from);
   BOOST_CHECK_EQUAL(to.serialize(), expected.serialize());
}

BOOST_AUTO_TEST_CASE(MergeAndRemoveSource)
{
   shared::CDataContainer to(
      "{"
      "   \"int\": 23,"
      "   \"string\": \"\""
      "}");

   shared::CDataContainer from(
      "{"
      "   \"int\": 135,"
      "   \"string\": \"1234\""
      "}");

   const shared::CDataContainer expected(
      "{"
      "   \"int\": 135,"
      "   \"string\": \"1234\""
      "}");


   to.mergeFrom(from);

   // Override from memory
   from = std::string("{ \"anotherdata\" : \"skdjflsjdfhgkdjhgxjkmflsdjfglsdjfmlklsmjdf\"}");

   BOOST_CHECK_EQUAL(to.serialize(), expected.serialize());
}

BOOST_AUTO_TEST_CASE(CheckDataLimits)
{
   shared::CDataContainer c(
      "{"
      "   \"char_min\": -128,"
      "   \"char_max\": 127,"
      "   \"uchar_max\": 255,"
      "   \"short_min\": -32768,"
      "   \"short_max\": 32767,"
      "   \"ushort_max\": 65535,"
      "   \"int_min\": -2147483648,"
      "   \"int_max\": 2147483647,"
      "   \"uint_max\": 4294967295,"
      "   \"int64_min\": -9223372036854775808,"
      "   \"int64_max\": 9223372036854775807,"
      "   \"uint64_max\": 18446744073709551615"
      "}");

   //check some out of range

   BOOST_CHECK_EQUAL(c.get<char>("char_min"), INT8_MIN);
   BOOST_CHECK_EQUAL(c.get<char>("char_max"), INT8_MAX);
   BOOST_CHECK_THROW(c.get<char>("uchar_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("short_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_THROW(c.get<unsigned char>("char_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_THROW(c.get<unsigned char>("short_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<short>("char_min"), INT8_MIN);
   BOOST_CHECK_EQUAL(c.get<short>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<short>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<short>("short_min"), INT16_MIN);
   BOOST_CHECK_EQUAL(c.get<short>("short_max"), INT16_MAX);
   BOOST_CHECK_THROW(c.get<short>("ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_THROW(c.get<unsigned short>("char_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_THROW(c.get<unsigned short>("short_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("ushort_max"), UINT16_MAX);
   BOOST_CHECK_THROW(c.get<unsigned short>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<int>("char_min"), INT8_MIN);
   BOOST_CHECK_EQUAL(c.get<int>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("short_min"), INT16_MIN);
   BOOST_CHECK_EQUAL(c.get<int>("short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("int_min"), INT32_MIN);
   BOOST_CHECK_EQUAL(c.get<int>("int_max"), INT32_MAX);
   BOOST_CHECK_THROW(c.get<int>("uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_THROW(c.get<unsigned int>("char_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("char_max"), static_cast<unsigned int>(INT8_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_THROW(c.get<unsigned int>("short_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("short_max"), static_cast<unsigned int>(INT16_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("ushort_max"), UINT16_MAX);
   BOOST_CHECK_THROW(c.get<unsigned int>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("int_max"), static_cast<unsigned int>(INT32_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("uint_max"), UINT32_MAX);
   BOOST_CHECK_THROW(c.get<unsigned int>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned int>("int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned int>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<int64_t>("char_min"), INT8_MIN);
   BOOST_CHECK_EQUAL(c.get<int64_t>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("short_min"), INT16_MIN);
   BOOST_CHECK_EQUAL(c.get<int64_t>("short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("int_min"), INT32_MIN);
   BOOST_CHECK_EQUAL(c.get<int64_t>("int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("uint_max"), UINT32_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("int64_min"), INT64_MIN);
   BOOST_CHECK_EQUAL(c.get<int64_t>("int64_max"), INT64_MAX);
   BOOST_CHECK_THROW(c.get<int64_t>("uint64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_THROW(c.get<uint64_t>("char_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("uchar_max"), UINT8_MAX);
   BOOST_CHECK_THROW(c.get<uint64_t>("short_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("ushort_max"), UINT16_MAX);
   BOOST_CHECK_THROW(c.get<uint64_t>("int_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("uint_max"), UINT32_MAX);
   BOOST_CHECK_THROW(c.get<uint64_t>("int64_min"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("int64_max"), static_cast<uint64_t>(INT64_MAX));
   BOOST_CHECK_EQUAL(c.get<uint64_t>("uint64_max"), UINT64_MAX);
}

BOOST_AUTO_TEST_CASE(CheckDataLimitsFloatingPrecision)
{
   shared::CDataContainer c(
      "{"
      "   \"double\": 42.0,"
      "   \"float\": 42.0,"
      "   \"double2\": 42.1,"
      "   \"float2\": 42.1,"
      "   \"gt_uchar_max\": 255.1,"
      "   \"gt_char_max\": 127.1,"
      "   \"eq_char_max\": 127.0,"
      "   \"gt_ushort_max\": 65535.1,"
      "   \"gt_short_max\": 32767.1,"
      "   \"eq_short_max\": 32767.0,"
      "   \"gt_uint_max\": 4294967295.1,"
      "   \"gt_int_max\": 2147483647.1,"
      "   \"eq_int_max\": 2147483647.0,"
      "   \"gt_uint64_max\": 18446744073709551615.1,"
      "   \"gt_int64_max\": 9223372036854775807.1,"
      "   \"eq_int64_max\": 9223372036854775807.0"
      "}");

   //check some out of range
   BOOST_CHECK_EQUAL(c.get<char>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<char>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<char>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<char>("float2"), 42);
   BOOST_CHECK_THROW(c.get<char>("gt_uchar_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_char_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<char>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_THROW(c.get<char>("gt_ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("eq_short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("eq_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<char>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<unsigned char>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("float2"), 42);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_uchar_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned char>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("eq_short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("eq_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned char>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<short>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<short>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<short>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<short>("float2"), 42);
   BOOST_CHECK_EQUAL(c.get<short>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<short>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<short>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_THROW(c.get<short>("gt_ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("gt_short_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<short>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_THROW(c.get<short>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("gt_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("eq_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<short>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<unsigned short>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("float2"), 42);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_THROW(c.get<unsigned short>("gt_ushort_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("gt_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned short>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_THROW(c.get<unsigned short>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("gt_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("eq_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned short>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<int>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<int>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<int>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<int>("float2"), 42);
   BOOST_CHECK_EQUAL(c.get<int>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("gt_ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("gt_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_THROW(c.get<int>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("gt_int_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<int>("eq_int_max"), INT32_MAX);
   BOOST_CHECK_THROW(c.get<int>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<unsigned int>("double"), static_cast<unsigned int>(42));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("float"), static_cast<unsigned int>(42));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("double2"), static_cast<unsigned int>(42));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("float2"), static_cast<unsigned int>(42));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("gt_char_max"), static_cast<unsigned int>(INT8_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("eq_char_max"), static_cast<unsigned int>(INT8_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("gt_ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("gt_short_max"), static_cast<unsigned int>(INT16_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("eq_short_max"), static_cast<unsigned int>(INT16_MAX));
   BOOST_CHECK_THROW(c.get<unsigned int>("gt_uint_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<unsigned int>("gt_int_max"), static_cast<unsigned int>(INT32_MAX));
   BOOST_CHECK_EQUAL(c.get<unsigned int>("eq_int_max"), static_cast<unsigned int>(INT32_MAX));
   BOOST_CHECK_THROW(c.get<unsigned int>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned int>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<unsigned int>("eq_int64_max"), shared::exception::COutOfRange);

   BOOST_CHECK_EQUAL(c.get<int64_t>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<int64_t>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<int64_t>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<int64_t>("float2"), 42);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_uint_max"), UINT32_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("gt_int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<int64_t>("eq_int_max"), INT32_MAX);
   BOOST_CHECK_THROW(c.get<int64_t>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int64_t>("gt_int64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_THROW(c.get<int64_t>("eq_int64_max"), shared::exception::COutOfRange); //int64 max with .0 is overflowed

   BOOST_CHECK_EQUAL(c.get<uint64_t>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("double2"), 42);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("float2"), 42);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_uchar_max"), UINT8_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_ushort_max"), UINT16_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_uint_max"), UINT32_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("eq_int_max"), INT32_MAX);
   BOOST_CHECK_THROW(c.get<uint64_t>("gt_uint64_max"), shared::exception::COutOfRange);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("gt_int64_max"), 9223372036854775807.1);
   BOOST_CHECK_EQUAL(c.get<uint64_t>("eq_int64_max"), 9223372036854775807.1); //int64 max with .0 is overflowed

   BOOST_CHECK_EQUAL(c.get<double>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<double>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<double>("double2"), 42.1);
   BOOST_CHECK_EQUAL(c.get<double>("float2"), 42.1);
   BOOST_CHECK_EQUAL(c.get<double>("gt_uchar_max"), 255.1);
   BOOST_CHECK_EQUAL(c.get<double>("gt_char_max"), 127.1);
   BOOST_CHECK_EQUAL(c.get<double>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<double>("gt_ushort_max"), 65535.1);
   BOOST_CHECK_EQUAL(c.get<double>("gt_short_max"), 32767.1);
   BOOST_CHECK_EQUAL(c.get<double>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<double>("gt_uint_max"), 4294967295.1);
   BOOST_CHECK_EQUAL(c.get<double>("gt_int_max"), 2147483647.1);
   BOOST_CHECK_EQUAL(c.get<double>("eq_int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<double>("gt_uint64_max"), 18446744073709551615.1);
   BOOST_CHECK_EQUAL(c.get<double>("gt_int64_max"), 9223372036854775807.1);
   BOOST_CHECK_EQUAL(c.get<double>("eq_int64_max"), INT64_MAX);

   BOOST_CHECK_EQUAL(c.get<float>("double"), 42);
   BOOST_CHECK_EQUAL(c.get<float>("float"), 42);
   BOOST_CHECK_EQUAL(c.get<float>("double2"), 42.1f);
   BOOST_CHECK_EQUAL(c.get<float>("float2"), 42.1f);
   BOOST_CHECK_EQUAL(c.get<float>("gt_uchar_max"), 255.1f);
   BOOST_CHECK_EQUAL(c.get<float>("gt_char_max"), 127.1f);
   BOOST_CHECK_EQUAL(c.get<float>("eq_char_max"), INT8_MAX);
   BOOST_CHECK_EQUAL(c.get<float>("gt_ushort_max"), 65535.1f);
   BOOST_CHECK_EQUAL(c.get<float>("gt_short_max"), 32767.1f);
   BOOST_CHECK_EQUAL(c.get<float>("eq_short_max"), INT16_MAX);
   BOOST_CHECK_EQUAL(c.get<float>("gt_uint_max"), 4294967295.1f);
   BOOST_CHECK_EQUAL(c.get<float>("gt_int_max"), 2147483647.1f);
   BOOST_CHECK_EQUAL(c.get<float>("eq_int_max"), INT32_MAX);
   BOOST_CHECK_EQUAL(c.get<float>("gt_uint64_max"), 18446744073709551615.1f);
   BOOST_CHECK_EQUAL(c.get<float>("gt_int64_max"), 9223372036854775807.1f);
   BOOST_CHECK_EQUAL(c.get<float>("eq_int64_max"), INT64_MAX);

}

BOOST_AUTO_TEST_CASE(MergeOldAndNewJsonStyle)
{
   shared::CDataContainer to(
      "{"
      "   \"developerMode\": false,"
      "   \"location\":"
      "   {"
      "      \"latitude\": 48.853,"
      "      \"longitude\": \"2.35\","
      "      \"timezone\": \"Europe/Paris\""
      "   },"
      "   \"language\": \"en\""
      "}");

   shared::CDataContainer from(
      "{"
      "   \"developerMode\": \"true\","
      "   \"location\":"
      "   {"
      "      \"latitude\": \"52.5\""
      "   },"
      "   \"language\": \"fr\","
      "   \"newBloc\":"
      "   {"
      "      \"valueLevel1\": 1,"
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": 2,"
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   }"
      "}");

   const shared::CDataContainer expected(
      "{"
      "   \"developerMode\": true,"
      "   \"location\":"
      "   {"
      "      \"latitude\": 52.5,"
      "      \"longitude\": \"2.35\","
      "      \"timezone\": \"Europe/Paris\""
      "   },"
      "   \"language\": \"fr\","
      "   \"newBloc\":"
      "   {"
      "      \"valueLevel1\": 1,"
      "      \"level2\":"
      "      {"
      "         \"valueLevel2\": 2,"
      "         \"level3\": "
      "         {"
      "            \"valueLevel3\": \"abc\""
      "         }"
      "      }"
      "   }"
      "}");


   to.mergeFrom(from);
   BOOST_CHECK_EQUAL(to.serialize(), expected.serialize());
}


template <typename T>
void CHECK_MAPS(const std::map<std::string, T>& input, const std::map<std::string, T>& output)
{
   BOOST_CHECK_EQUAL(input.size(), output.size());
   std::map<std::string, T>::const_iterator io = output.begin();
   for (const auto& ii : input)
   {
      BOOST_CHECK_EQUAL(ii.first, io->first);
      BOOST_CHECK_EQUAL(ii.second, io->second);
      ++io;
   }
}

BOOST_AUTO_TEST_CASE(Map)
{
   std::map<std::string, std::string> input = { {"key1", "value1"},{"key2", "value2"},{"key3", "value3"},{"key4", "value4"} };
   shared::CDataContainer dc(input);

   auto output = dc.getAsMap<std::string>();

   //don't use BOOST_CHECK_EQUAL_COLLECTIONS because it do not builds with std::map
   CHECK_MAPS<std::string>(input, output);

   auto output2 = dc.get<std::map<std::string, std::string>>();
   CHECK_MAPS<std::string>(input, output2);
}

BOOST_AUTO_TEST_CASE(MapOfContainers)
{
   const shared::CDataContainer input("\
      {\
         \"0\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0\"\
         },\
         \"1\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.2\"\
         },\
         \"2\": {\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0-rc.1\"\
         }\
      }");

   const std::map<std::string, shared::CDataContainer> expected =
   {
      {"0", shared::CDataContainer("{\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0\"\
         }")},
         {"1", shared::CDataContainer("{\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.2\"\
         }")},
         {"2", shared::CDataContainer("{\
            \"name\": \"moon\",\
            \"description\": null,\
            \"version\": \"1.0.0-rc.1\"\
         }")}
   };

   CHECK_MAPS<shared::CDataContainer>(input.getAsMap<shared::CDataContainer>(), expected);
}

BOOST_AUTO_TEST_SUITE_END()
