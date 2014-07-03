#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"
#include <shared/Log.h>

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
   YADOMS_LOG(debug) <<"[START] Testing shared::CDataContainer : simple container" ;

   shared::CDataContainer dc;

   boost::posix_time::ptime actualDatetime = boost::posix_time::second_clock::universal_time();

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
   BOOST_CHECK_EQUAL(test.get<double>("double"),4.0);
   BOOST_CHECK_EQUAL(test.get<std::string>("string"), "plouf");
   
   shared::CDataContainer subContainer;
   subContainer.set<int>("int4", 6);
   subContainer.set<double>("double1", 8.0);
   subContainer.set<std::string>("string2", "plouf2");

   test.set("config1", subContainer);

   BOOST_CHECK_EQUAL(test.get<double>("config1.double1"), 8.0);


   //check for shared_ptr
   boost::shared_ptr<std::string> shs(new std::string("un shared ptr"));
   dc.set("StringSharedPtr", shs);

   boost::shared_ptr<std::string> shs2 = dc.get< boost::shared_ptr<std::string> >("StringSharedPtr");
   BOOST_CHECK_EQUAL(*(shs.get()) == *(shs2.get()), true);

   YADOMS_LOG(debug) <<"[END] Testing shared::CDataContainer : simple container" ;
}

BOOST_AUTO_TEST_CASE(CollectionContainer)
{
   YADOMS_LOG(debug) <<"[START] Testing shared::CDataContainer : collection container" ;

   shared::CDataContainer test;

   //check vector of int
   std::vector<int> vi;
   for (int i = 0; i<10; ++i)
      vi.push_back(i);
   test.set < std::vector<int> >("vectorint", vi);
   std::vector<int> vi2 = test.get<std::vector<int>>("vectorint");
   BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end());

   //check vector of double
   std::vector<double> vd;
   for (int i = 0; i<10; ++i)
      vd.push_back(i * 3.0);
   test.set< std::vector<double> >("vectordouble", vd);
   std::vector<double> vd2 = test.get< std::vector<double> >("vectordouble");
   BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end());

   //check vector of shared_ptr<int>
   std::vector< boost::shared_ptr<int> > vish;
   for (int i = 0; i < 10; ++i)
      vish.push_back(boost::shared_ptr<int>(new int(i)));
   test.set("vectorintsh", vish);
   std::vector< boost::shared_ptr<int> > vish2 = test.get< std::vector< boost::shared_ptr<int> > >("vectorintsh");
   std::vector< int > vish2bis = test.get< std::vector< int > >("vectorintsh");
   BOOST_CHECK_EQUAL(vish.size(), vish2.size());
   for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == *(vish2[i].get()), true);

   BOOST_CHECK_EQUAL(vish.size(), vish2bis.size());
   for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == vish2bis[i], true);

   //check vector of shared_ptr<double>
   std::vector< boost::shared_ptr<double> > vdsh;
   for (unsigned int i = 0; i < 10; ++i)
      vdsh.push_back(boost::shared_ptr<double>(new double(i*42.0)));
   test.set("vectordoublesh", vdsh);
   std::vector< boost::shared_ptr<double> > vdsh2 = test.get< std::vector< boost::shared_ptr<double> > >("vectordoublesh");
   std::vector< double > vdsh2bis = test.get< std::vector< double > >("vectordoublesh");
   BOOST_CHECK_EQUAL(vdsh.size(), vdsh2.size());
   for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == *(vdsh2[i].get()), true);

   BOOST_CHECK_EQUAL(vdsh.size(), vdsh2bis.size());
   for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == vdsh2bis[i], true);

   YADOMS_LOG(debug) <<"[END] Testing shared::CDataContainer : collection container" ;
}

BOOST_AUTO_TEST_CASE(Serialization)
{
   YADOMS_LOG(debug) <<"[START] Testing shared::CDataContainer : serialization" ;

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
   YADOMS_LOG(debug) <<"[END] Testing shared::CDataContainer : serialization" ;
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
	
	bool equals(const CTestClass & rhs)
	{
		return m_aIntValue == rhs.m_aIntValue && m_dValue == rhs.m_dValue && m_sValue == rhs.m_sValue;
	}

	bool operator!=(const CTestClass &rhs)
	{
		return !equals(rhs);
	}

   friend std::ostream& operator << (std::ostream& ostr, CTestClass const& str)
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
   YADOMS_LOG(debug) <<"[START] Testing shared::CDataContainer : shared::IDataContainable" ;
	//containeur simple de IDataContainable
	CTestClass obj(1, 42.0, "test de datacontainble");
	shared::CDataContainer cont;
	cont.set("myobject", obj);
	CTestClass result = cont.get<CTestClass>("myobject");
	BOOST_CHECK_EQUAL(obj.equals(result), true);

	//containeur de boost::shared_ptr<IDataContainable>
	boost::shared_ptr<CTestClass> sp(new CTestClass(2, 43.0, "chaine1"));
	shared::CDataContainer cont2;
	cont2.set("myobject", sp);
	boost::shared_ptr<CTestClass> result2 = cont2.get< boost::shared_ptr<CTestClass> >("myobject");
   CTestClass result2bis = cont2.get<CTestClass>("myobject");
	BOOST_CHECK_EQUAL(result2->equals(*sp.get()), true);
   BOOST_CHECK_EQUAL(result2bis.equals(*sp.get()), true);

	//containeur simple de std::vector<IDataContainable>
	std::vector<CTestClass> vc;
	for (int i = 0; i < 10; ++i)
		vc.push_back(CTestClass(i, 42.0 * i, "test de std::vector<IDataContainable>"));
	shared::CDataContainer contvec;
	contvec.set("mycollection", vc);
	std::vector<CTestClass> vc2 = contvec.get< std::vector<CTestClass> >("mycollection");
   BOOST_CHECK_EQUAL_COLLECTIONS(vc.begin(), vc.end(), vc2.begin(), vc2.end());

   //containeur simple de std::vector< boost::shared_ptr<IDataContainable> >
   std::vector< boost::shared_ptr<CTestClass> > vcsh;
   for (int i = 0; i < 10; ++i)
      vcsh.push_back(boost::shared_ptr<CTestClass>(new CTestClass(i, 42.0 * i, "test de std::vector<IDataContainable>")));
   shared::CDataContainer contvecsh;
   contvecsh.set("mycollectionofshared", vcsh);
   std::vector< boost::shared_ptr<CTestClass> >  vcsh2 = contvecsh.get< std::vector< boost::shared_ptr<CTestClass> > >("mycollectionofshared");
   std::vector< CTestClass >  vc2bis = contvecsh.get< std::vector< CTestClass > >("mycollectionofshared");
   BOOST_CHECK_EQUAL(vcsh.size(), vcsh2.size());
   for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(*vcsh2[i].get()), true);

   BOOST_CHECK_EQUAL(vcsh.size(), vc2bis.size());
   for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(vc2bis[i]), true);

   YADOMS_LOG(debug) <<"[END] Testing shared::CDataContainer : shared::IDataContainable" ;

}

BOOST_AUTO_TEST_SUITE_END()
