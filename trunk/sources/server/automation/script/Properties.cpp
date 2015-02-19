#include "stdafx.h"
#include "Properties.h"

namespace automation { namespace script
{

CProperties::CProperties(boost::shared_ptr<const database::entities::CRule> ruleData)
   :m_interpreterName(ruleData->Type()),
   m_modelBased(!ruleData->Model().empty()),
   m_scriptPath(buildScriptPath(ruleData)),
   m_configuration(ruleData->Configuration())
{
}

CProperties::~CProperties()
{         
}

std::string CProperties::buildScriptPath(boost::shared_ptr<const database::entities::CRule> ruleData)
{
   boost::filesystem::path scriptPath("scripts");

   if (m_modelBased)
   {
      scriptPath /= "templates";
      scriptPath /= ruleData->Name();
   }
   else
   {
      scriptPath /= "locals";
      scriptPath /= std::string("rule_") + boost::lexical_cast<std::string>(ruleData->Id());
   }

   return scriptPath.string();
}

bool CProperties::isModelBased() const
{
   return m_modelBased;
}

std::string CProperties::interpreterName() const
{
   return m_interpreterName;
}

std::string CProperties::scriptPath() const
{
   return m_scriptPath;
}

const shared::CDataContainer& CProperties::configuration() const
{
   return m_configuration;
}

} } // namespace automation::script

