#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/job/condition/And.h"
#include "../../../../sources/server/job/condition/IConditionFactory.h"

// Includes needed to compile the test
#include "../../../testCommon/fileSystem.h"

BOOST_AUTO_TEST_SUITE(TestAnd)

class CConditionFactoryMocke : public job::condition::IConditionFactory
{
public:
   CConditionFactoryMocke() {}
   virtual ~CConditionFactoryMocke() {}
   virtual boost::shared_ptr<job::condition::ICondition> createCondition(const shared::CDataContainer& configuration) const { return boost::shared_ptr<job::condition::ICondition>(); }
};

//--------------------------------------------------------------
/// \brief	    Nominal case
//--------------------------------------------------------------
BOOST_AUTO_TEST_CASE(Nominal)
{
   CConditionFactoryMocke factory;
   const std::vector<shared::CDataContainer> andOperands;//TODO
   boost::shared_ptr<job::condition::ICondition> and(new job::condition::CAnd(andOperands, factory));

   BOOST_CHECK_EQUAL(and->eval(), true);
}
//TODO add tests
BOOST_AUTO_TEST_SUITE_END()