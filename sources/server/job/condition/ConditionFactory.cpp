#include "stdafx.h"
#include <shared/Log.h>
#include "ConditionFactory.h"
#include "Empty.h"
#include "Is.h"
#include "And.h"

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

   if (!condition)
   {
      YADOMS_LOG(error) << "Invalid job condition configuration : " << configuration.serialize();
      YADOMS_LOG(error) << "data : " << configuration.serialize();
   }

   return condition;
}

} } // namespace job::condition	
	
	