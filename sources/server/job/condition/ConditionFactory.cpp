#include "stdafx.h"
#include <shared/Log.h>
#include "ConditionFactory.h"
#include "Empty.h"
#include "Is.h"
#include "And.h"
#include "Not.h"
#include "Or.h"

namespace job { namespace condition
{

CConditionFactory::CConditionFactory(boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_dbAcquisitionRequester(dbAcquisitionRequester)
{
}

CConditionFactory::~CConditionFactory()
{         
}

boost::shared_ptr<ICondition> CConditionFactory::createCondition(const shared::CDataContainer& configuration) const
{
   boost::shared_ptr<ICondition> condition;

   if (configuration.empty())
   {
      condition.reset(new CEmpty);
   }
   else if (configuration.hasValue("is"))
   {
      condition.reset(new CIs(configuration.get<shared::CDataContainer>("is"), m_dbAcquisitionRequester));
   }
   else if (configuration.hasValue("and"))
   {
      condition.reset(new CAnd(configuration.get<std::vector<shared::CDataContainer> >("and"), *this));
   }
   else if (configuration.hasValue("or"))
   {
      condition.reset(new COr(configuration.get<std::vector<shared::CDataContainer> >("or"), *this));
   }
   else if (configuration.hasValue("not"))
   {
      condition.reset(new CNot(configuration.get<shared::CDataContainer>("not"), *this));
   }

   if (!condition)
   {
      YADOMS_LOG(error) << "Invalid job condition configuration : " << configuration.serialize();
      YADOMS_LOG(error) << "data : " << configuration.serialize();
   }

   return condition;
}

} } // namespace job::condition	
	
	