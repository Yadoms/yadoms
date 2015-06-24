#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
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

BOOST_AUTO_TEST_SUITE(TestEnum)



BOOST_AUTO_TEST_CASE(Enums)
{
	database::entities::ESecurityAccess ev;
	

   //check data are correctly retreived
   ev = database::entities::ESecurityAccess::kNone;
   BOOST_CHECK_EQUAL(ev, database::entities::ESecurityAccess::kNone);
   BOOST_CHECK_EQUAL(ev, (int)database::entities::ESecurityAccess::kNone);
   BOOST_CHECK(boost::iequals((std::string)ev, "None"));
	BOOST_CHECK(boost::iequals(ev.getAsString(), "None"));   
   
   ev = 2;
   BOOST_CHECK_EQUAL(ev, 2);
   BOOST_CHECK_EQUAL(ev, database::entities::ESecurityAccess::kUser);
   BOOST_CHECK(boost::iequals((std::string)ev, "User"));
	BOOST_CHECK(boost::iequals(ev.getAsString(), "User"));
   
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

   

}


BOOST_AUTO_TEST_SUITE_END()
