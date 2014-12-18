#include "stdafx.h"
#include <shared/Log.h>
#include "ConditionFactory.h"
#include "Empty.h"
#include "Is.h"
#include "And.h"

namespace job { namespace condition
{

CConditionFactory::CConditionFactory()
{
}

CConditionFactory::~CConditionFactory()
{         
}

boost::shared_ptr<ICondition> CConditionFactory::createCondition(const shared::CDataContainer& configuration) const
{
   boost::shared_ptr<ICondition> condition;

   try
   {
      if (configuration.empty())
      {
         condition.reset(new CEmpty);
      }
      else if (configuration.hasValue("is"))
      {
         condition.reset(new CIs(configuration.get<shared::CDataContainer>("is")));
      }
      else if (configuration.hasValue("and"))
      {
         condition.reset(new CAnd(configuration.get<std::vector<shared::CDataContainer> >("and"), *this));
      }
      // TODO add other conditions here

      if (!condition)
      {
         YADOMS_LOG(error) << "Invalid job condition configuration : " << configuration.serialize();
         YADOMS_LOG(error) << "data : " << configuration.serialize();
      }
   }
   catch(shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Invalid job condition configuration, invalid parameter : " << e.what();
      YADOMS_LOG(error) << "data : " << configuration.serialize();
   }
   catch(shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Invalid job condition configuration, out of range : " << e.what();
      YADOMS_LOG(error) << "data : " << configuration.serialize();
   }

   return condition;
}

} } // namespace job::condition	
	
	