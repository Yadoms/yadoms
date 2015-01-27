#include "stdafx.h"
#include <shared/Log.h>
#include "ConditionFactory.h"
#include "Is.h"
#include "IsFor.h"
#include "And.h"
#include "Not.h"
#include "Or.h"

namespace automation { namespace condition
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
      return condition;

   if (configuration.hasValue("is"))
   {
      const shared::CDataContainer& isConfiguration(configuration.get<shared::CDataContainer>("is"));
      if (isConfiguration.hasValue("for"))
         condition.reset(new CIsFor(isConfiguration, m_dbAcquisitionRequester));
      else
         condition.reset(new CIs(isConfiguration, m_dbAcquisitionRequester));
      return condition;
   }
   
   if (configuration.hasValue("and"))
   {
      condition.reset(new CAnd(configuration.get<std::vector<shared::CDataContainer> >("and"), *this));
      return condition;
   }
   
   if (configuration.hasValue("or"))
   {
      condition.reset(new COr(configuration.get<std::vector<shared::CDataContainer> >("or"), *this));
      return condition;
   }
   
   if (configuration.hasValue("not"))
   {
      condition.reset(new CNot(configuration.get<shared::CDataContainer>("not"), *this));
      return condition;
   }

   YADOMS_LOG(error) << "Invalid rule condition configuration : " << configuration.serialize();
   YADOMS_LOG(error) << "data : " << configuration.serialize();

   return condition;
}

} } // namespace automation::condition	
	
	