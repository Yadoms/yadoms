#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"

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


   BOOST_AUTO_TEST_CASE(SimpleContainer)
   {
      shared::CDataContainer dc;

      auto actualDatetime = boost::posix_time::second_clock::universal_time();

      //ensure en new container is empty
      BOOST_CHECK_EQUAL(dc.empty(), true) ;

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
      BOOST_CHECK_EQUAL(dc.get<bool>("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(dc.get<double>("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(dc.get<EEnumType>("EnumParameter"), kEnumValue2) ;
      BOOST_CHECK_EQUAL(dc.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1) ;
      BOOST_CHECK_EQUAL(dc.get<int>("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(dc.get<int>("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;
      BOOST_CHECK_EQUAL(dc.get<boost::posix_time::ptime>("DateTimeParameter"), actualDatetime) ;

      //another test for a sub container
      shared::CDataContainer test;
      test.set<int>("int", 5);
      test.set<double>("double", 4.0);
      test.set<std::string>("string", "plouf");

      BOOST_CHECK_EQUAL(test.get<int>("int"), 5) ;
      BOOST_CHECK_EQUAL(test.get<double>("double"),4.0) ;
      BOOST_CHECK_EQUAL(test.get<std::string>("string"), "plouf") ;

      shared::CDataContainer subContainer;
      subContainer.set<int>("int4", 6);
      subContainer.set<double>("double1", 8.0);
      subContainer.set<std::string>("string2", "plouf2");

      test.set("config1", subContainer);

      BOOST_CHECK_EQUAL(test.get<double>("config1.double1"), 8.0) ;


      //check for shared_ptr
      auto shs(boost::make_shared<std::string>("un shared ptr"));
      dc.set("StringSharedPtr", shs);

      auto shs2 = dc.get<boost::shared_ptr<std::string>>("StringSharedPtr");
      BOOST_CHECK_EQUAL(*(shs.get()) == *(shs2.get()), true) ;
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
      BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end()) ;

      //check vector of double
      std::vector<double> vd;
      for (auto i = 0; i < 10; ++i)
         vd.push_back(i * 3.0);
      test.set<std::vector<double>>("vectordouble", vd);
      auto vd2 = test.get<std::vector<double>>("vectordouble");
      BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end()) ;

      //check vector of EEnumType
      std::vector<EEnumType> ve;
      ve.push_back(kEnumValue2);
      ve.push_back(kEnumValue4);
      ve.push_back(kEnumValue5);
      ve.push_back(kEnumValue7);
      test.set("vectorenum", ve);
      std::vector<EEnumType> ve2 = test.get<std::vector<EEnumType>>("vectorenum");
      BOOST_CHECK_EQUAL_COLLECTIONS(ve.begin(), ve.end(), ve2.begin(), ve2.end()) ;

      //check vector of shared_ptr<int>
      std::vector<boost::shared_ptr<int>> vish;
      for (auto i = 0; i < 10; ++i)
         vish.push_back(boost::make_shared<int>(i));
      test.set("vectorintsh", vish);
      auto vish2 = test.get<std::vector<boost::shared_ptr<int>>>("vectorintsh");
      auto vish2bis = test.get<std::vector<int>>("vectorintsh");
      BOOST_CHECK_EQUAL(vish.size(), vish2.size()) ;
      for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == *(vish2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vish.size(), vish2bis.size()) ;
      for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == vish2bis[i], true) ;

      //check vector of shared_ptr<double>
      std::vector<boost::shared_ptr<double>> vdsh;
      for (unsigned int i = 0; i < 10; ++i)
         vdsh.push_back(boost::make_shared<double>(i * 42.0));
      test.set("vectordoublesh", vdsh);
      auto vdsh2 = test.get<std::vector<boost::shared_ptr<double>>>("vectordoublesh");
      auto vdsh2bis = test.get<std::vector<double>>("vectordoublesh");
      BOOST_CHECK_EQUAL(vdsh.size(), vdsh2.size()) ;
      for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == *(vdsh2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vdsh.size(), vdsh2bis.size()) ;
      for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == vdsh2bis[i], true) ;

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

      BOOST_CHECK_EQUAL(allconditions.size(), getAllCond.size()) ;

      auto getCond1 = getAllCond[0];
      auto getCond2 = getAllCond[1];
      auto getCond3 = getAllCond[2];

      BOOST_CHECK_EQUAL(cond1, getCond1) ;
      BOOST_CHECK_EQUAL(cond2, getCond2) ;
      BOOST_CHECK_EQUAL(cond3, getCond3) ;

      BOOST_CHECK_EQUAL_COLLECTIONS(allconditions.begin(), allconditions.end(), getAllCond.begin(), getAllCond.end()) ;
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

      BOOST_CHECK_EQUAL(cfg.get<bool>("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.get<double>("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(cfg.get<int>("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(cfg.get<EEnumType>("EnumParameter"), kEnumValue2) ;
      BOOST_CHECK_EQUAL(cfg.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1) ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(cfg.get<int>("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;

      boost::posix_time::ptime expected(boost::gregorian::date(2014, 7, 2), boost::posix_time::hours(11) + boost::posix_time::minutes(35) + boost::posix_time::seconds(0));
      BOOST_CHECK_EQUAL(cfg.get<boost::posix_time::ptime>("DateTimeParameter"), expected) ;

      //check that serialization match original values
      //just remove space, \n, \t and \r from strings
      auto str = cfg.serialize();
      auto str2 = defaultConf;

      str.erase(std::remove_if(str.begin(), str.end(), boost::is_any_of(" \t\n\r")), str.end());
      str2.erase(std::remove_if(str2.begin(), str2.end(), boost::is_any_of(" \t\n\r")), str2.end());

      BOOST_CHECK_EQUAL(str, str2) ;


      std::stringstream ss;
      ss << cfg;

      shared::CDataContainer cfgOut;
      ss >> cfgOut;

      BOOST_CHECK_EQUAL(cfg.serialize(), cfgOut.serialize()) ;

      //YADOMS_LOG(information) << cfg;
   }


   BOOST_AUTO_TEST_CASE(CheckExistance)
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

      //check path existance
      BOOST_CHECK_EQUAL(cfg.exists(""), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection.SubIntParameter"), true) ;

      BOOST_CHECK_EQUAL(cfg.exists("SubIntParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection2"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("String Parameter"), false) ;

      //check child existance
      BOOST_CHECK_EQUAL(cfg.containsChild(""), true) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("BoolParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection.SubIntParameter"), false) ;

      //check value existance
      BOOST_CHECK_EQUAL(cfg.containsValue(""), false) ;
      BOOST_CHECK_EQUAL(cfg.containsValue("MySection"), false) ;
      BOOST_CHECK_EQUAL(cfg.containsValue("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.containsValue("MySection.SubIntParameter"), true) ;
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
      BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms"), true) ;
      auto supportedPf = testPf.get<shared::CDataContainer>("supportedPlatforms");
      BOOST_CHECK_EQUAL(supportedPf.containsChild(), true) ;
      BOOST_CHECK_EQUAL(supportedPf.containsValue(), false) ;
      BOOST_CHECK_EQUAL(supportedPf.get<std::string>(), "") ; //it do not contains value, only childs
      BOOST_CHECK_EQUAL(supportedPf.get<std::string>("mac"), "none") ;
      BOOST_CHECK_EQUAL(supportedPf.get<std::string>("raspberry"), "all") ;

      //value test
      BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms2"), true) ;
      auto supportedPf2 = testPf.get<shared::CDataContainer>("supportedPlatforms2");
      BOOST_CHECK_EQUAL(supportedPf2.containsChild(), false) ;
      BOOST_CHECK_EQUAL(supportedPf2.containsValue(), true) ;
      BOOST_CHECK_EQUAL(supportedPf2.get<std::string>(), "all") ;
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
         shared::CDataContainer deserializeData(serializedData);
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
      BOOST_CHECK_EQUAL(obj.equals(result), true) ;

      //containeur de boost::shared_ptr<IDataContainable>
      auto sp(boost::make_shared<CTestClass>(2, 43.0, "string1"));
      shared::CDataContainer cont2;
      cont2.set("myobject", sp);
      auto result2 = cont2.get<boost::shared_ptr<CTestClass>>("myobject");
      auto result2bis = cont2.get<CTestClass>("myobject");
      BOOST_CHECK_EQUAL(result2->equals(*sp.get()), true) ;
      BOOST_CHECK_EQUAL(result2bis.equals(*sp.get()), true) ;

      //containeur simple de std::vector<IDataContainable>
      std::vector<CTestClass> vc;
      for (auto i = 0; i < 10; ++i)
         vc.push_back(CTestClass(i, 42.0 * i, "test of std::vector<IDataContainable>"));
      shared::CDataContainer contvec;
      contvec.set("mycollection", vc);
      auto vc2 = contvec.get<std::vector<CTestClass>>("mycollection");
      BOOST_CHECK_EQUAL_COLLECTIONS(vc.begin(), vc.end(), vc2.begin(), vc2.end()) ;

      //containeur simple de std::vector< boost::shared_ptr<IDataContainable> >
      std::vector<boost::shared_ptr<CTestClass>> vcsh;
      for (auto i = 0; i < 10; ++i)
         vcsh.push_back(boost::make_shared<CTestClass>(i, 42.0 * i, "test of std::vector<IDataContainable>"));
      shared::CDataContainer contvecsh;
      contvecsh.set("mycollectionofshared", vcsh);
      auto vcsh2 = contvecsh.get<std::vector<boost::shared_ptr<CTestClass>>>("mycollectionofshared");
      auto vc2bis = contvecsh.get<std::vector<CTestClass>>("mycollectionofshared");
      BOOST_CHECK_EQUAL(vcsh.size(), vcsh2.size()) ;
      for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(*vcsh2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vcsh.size(), vc2bis.size()) ;
      for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(vc2bis[i]), true) ;
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
      BOOST_CHECK_EQUAL(dc.get<int>("FieldInt"), fi()) ;
      BOOST_CHECK_EQUAL(dc.get<double>("FieldDouble"), fd()) ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("FieldString"), fs()) ;
      BOOST_CHECK_EQUAL(dc.get<EEnumType>("FieldEnum"), fe()) ;
      BOOST_CHECK_EQUAL(dc.get<CTestClass>("FieldDataContainable").equals(fdc()), true) ;


      //vector de field
      std::vector<shared::CField<int>> vfi;
      for (auto i = 0; i < 10; ++i)
         vfi.push_back(shared::CField<int>(i));

      dc.set("VectorFieldInt", vfi);
      auto vi2 = dc.get<std::vector<int>>("VectorFieldInt");
      BOOST_CHECK_EQUAL_COLLECTIONS(vfi.begin(), vfi.end(), vi2.begin(), vi2.end()) ;
   }


   BOOST_AUTO_TEST_CASE(Path)
   {
      shared::CField<int> fi(10);

      //standard path separator using '.'
      shared::CDataContainer dc;
      dc.set("secA.secB.valC", fi);
      BOOST_CHECK_EQUAL(dc.get<int>("secA.secB.valC"), fi()) ;
      BOOST_CHECK_EQUAL(dc.get<shared::CDataContainer>("secA").get<shared::CDataContainer>("secB").get<int>("valC"), fi()) ;

      //no path using separator 0x00
      dc.set("secD.secE.valC", fi, 0x00);
      BOOST_CHECK_EQUAL(dc.get<int>("secD.secE.valC", 0x00), fi()) ;
      BOOST_CHECK_EQUAL(dc.exists("secD.secE.valC"), false) ;
      BOOST_CHECK_THROW(dc.get<int>("secD.secE.valC"), std::exception) ;
      BOOST_CHECK_THROW(dc.get<shared::CDataContainer>("secD"), std::exception) ;
   }


   BOOST_AUTO_TEST_CASE(SimpleConstruction)
   {
      BOOST_CHECK_THROW(shared::CDataContainer dc("1"), std::exception) ;
   }


#define BOOST_CHECK_MAPS(input, output) \
   { \
      BOOST_CHECK_EQUAL(input.size(), output.size());\
      std::map<std::string, std::string>::iterator ii, io;\
      io = output.begin();\
      for (ii = input.begin(); ii != input.end(); ++ii)\
      {\
         BOOST_CHECK_EQUAL(ii->first, io->first);\
         BOOST_CHECK_EQUAL(ii->second, io->second);\
         ++io;\
      }\
   }

   BOOST_AUTO_TEST_CASE(Map)
   {
      std::map<std::string, std::string> input = {{"key1", "value1"},{"key2", "value2"},{"key3", "value3"},{"key4", "value4"}};
      shared::CDataContainer dc(input);

      auto output = dc.getAsMap();

      //dont use BOOST_CHECK_EQUAL_COLLECTIONS because it do not builds with std::map
      BOOST_CHECK_MAPS(input, output);

      auto output2 = dc.get<std::map<std::string, std::string>>();
      BOOST_CHECK_MAPS(input, output2);
   }

   BOOST_AUTO_TEST_SUITE_END()

