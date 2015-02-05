#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/enumeration/EnumHelpers.hpp"
#include "../../../../sources/server/database/entities/Entities.h"
#include "../../../../sources/shared/shared/enumeration/IExtendedEnum.h"

template <typename T, class Enable = void>
struct testhelper
{
   static bool checkGoodType()
   {
      return false;
   }
};

template <typename T>
struct testhelper < T, typename boost::enable_if< boost::is_base_of< shared::enumeration::IExtendedEnum, T > >::type >
{
   static bool checkGoodType()
   {
      return true;
   }
};

enum EClassicEnum
{
   a=0,
   b=1,
};




class CFrameBase
{
public:
   //-----------------------------
   ///\brief Constructor
   ///\param [in]  content    A string that will be deserialized
   //-----------------------------
   CFrameBase(const std::string & content)
   {

   }

   //-----------------------------
   ///\brief Constructor
   ///\param [in]  type    The type of frame
   //-----------------------------
   CFrameBase(const database::entities::ESecurityAccess & type)
   {

   }
};


void testEnumAsParameter(database::entities::ESecurityAccess en)
{
}

void testEnumAsParameterWithTernaryOperator(database::entities::ESecurityAccess en, int i)
{
   if (i%2 ==0)
      BOOST_CHECK_EQUAL(en, database::entities::ESecurityAccess::kAdmin);
   else
      BOOST_CHECK_EQUAL(en, database::entities::ESecurityAccess::kUser);
}

BOOST_AUTO_TEST_SUITE(TestEnum)





BOOST_AUTO_TEST_CASE(Enums)
{
	database::entities::ESecurityAccess ev;
	
   //check data are correctly retreived
   ev = database::entities::ESecurityAccess::kNone;
   BOOST_CHECK_EQUAL(ev, database::entities::ESecurityAccess::kNone);
   BOOST_CHECK_EQUAL(ev, (int)database::entities::ESecurityAccess::kNone);
   BOOST_CHECK(boost::iequals((std::string)ev, "None"));
	BOOST_CHECK(boost::iequals(ev.toString(), "None"));   
   
   ev = 2;
   BOOST_CHECK_EQUAL(ev, 2);
   BOOST_CHECK_EQUAL(ev, database::entities::ESecurityAccess::kUser);
   BOOST_CHECK(boost::iequals((std::string)ev, "User"));
	BOOST_CHECK(boost::iequals(ev.toString(), "User"));
   
	BOOST_CHECK_NO_THROW(ev = "Admin");
	BOOST_CHECK_NO_THROW(ev = "None");
	BOOST_CHECK_NO_THROW(ev = "User");
	BOOST_CHECK_NO_THROW(ev = "admin");
	BOOST_CHECK_NO_THROW(ev = "none");
	BOOST_CHECK_NO_THROW(ev = "user");
	BOOST_CHECK_THROW(ev = "unknownvalue", std::exception);
	BOOST_CHECK_THROW(ev = "Admin ", std::exception);

   BOOST_CHECK_EQUAL(testhelper<database::entities::ESecurityAccess>::checkGoodType(), true);
   BOOST_CHECK_EQUAL(testhelper<EClassicEnum>::checkGoodType(), false);

   //check for method exist(int)
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined(0));
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined(1));
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined(2));
   BOOST_CHECK(!database::entities::ESecurityAccess::isDefined(-1));
   BOOST_CHECK(!database::entities::ESecurityAccess::isDefined(3));

   //check for method exist(std::string)
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined("none"));
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined("Admin"));
   BOOST_CHECK(database::entities::ESecurityAccess::isDefined("User"));
   BOOST_CHECK(!database::entities::ESecurityAccess::isDefined("plouf"));
   BOOST_CHECK(!database::entities::ESecurityAccess::isDefined("noneaaa"));

   //check for method parse(std::string)
   BOOST_CHECK(database::entities::ESecurityAccess::parse("none") == database::entities::ESecurityAccess::kNone);
   BOOST_CHECK(database::entities::ESecurityAccess::parse("Admin") == database::entities::ESecurityAccess::kAdmin);
   BOOST_CHECK(database::entities::ESecurityAccess::parse("User") == database::entities::ESecurityAccess::kUser);



   //check for switch/case
   switch (0)
   {
   case database::entities::ESecurityAccess::kNoneValue:
      break;
   case database::entities::ESecurityAccess::kAdminValue:
      break;
   case database::entities::ESecurityAccess::kUserValue:
      break;
   default:
      break;
   }
   
   
   



   //check CTOR
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess a);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess b0(0));
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess b1(1));
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess b2(2));
   BOOST_CHECK_THROW(database::entities::ESecurityAccess b3(3), std::exception);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess b4(database::entities::ESecurityAccess::kNone));

   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess c0("none"));
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess c1("Admin"));
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess c2("User"));
   BOOST_CHECK_THROW(database::entities::ESecurityAccess c3("failure"), std::exception);

   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess d0 = 0);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess d1 = 1);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess d2 = 2);
   BOOST_CHECK_THROW(database::entities::ESecurityAccess d3 = 3, std::exception);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess d4 = database::entities::ESecurityAccess::kAdmin);

   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess e0 = "none");
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess e1 = "Admin");
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess e2 = "User");
   BOOST_CHECK_THROW(database::entities::ESecurityAccess e3 = "failure", std::exception);
   BOOST_CHECK_NO_THROW(database::entities::ESecurityAccess e4 = database::entities::ESecurityAccess::kUser.toString());

   database::entities::ESecurityAccess f0(0);
   testEnumAsParameter(f0);
   testEnumAsParameter(database::entities::ESecurityAccess::kAdmin);
   testEnumAsParameter(1);
   testEnumAsParameter(std::string("admin"));
   testEnumAsParameter("admin");

   for (int i = 0; i < 10; i++)
   {
      testEnumAsParameterWithTernaryOperator(((i % 2) == 0) ? database::entities::ESecurityAccess::kAdmin : database::entities::ESecurityAccess::kUser, i);
   }

   CFrameBase g(database::entities::ESecurityAccess::kUser);
}


BOOST_AUTO_TEST_SUITE_END()
