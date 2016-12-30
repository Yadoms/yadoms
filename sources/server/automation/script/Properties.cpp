#include "stdafx.h"
#include "Properties.h"

namespace automation
{
   namespace script
   {
      CProperties::CProperties(boost::shared_ptr<const database::entities::CRule> ruleData)
         : m_interpreterName(ruleData->Interpreter()),
           m_modelBased(!ruleData->Model().empty()),
           m_scriptPath(buildScriptPath(ruleData)),
           m_logPath(buildLogPath()),
           m_configuration(ruleData->Configuration())
      {
      }

      CProperties::~CProperties()
      {
      }

      std::string CProperties::buildScriptPath(boost::shared_ptr<const database::entities::CRule> ruleData) const
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

      std::string CProperties::buildLogPath() const
      {
         auto logPath = m_scriptPath / boost::filesystem::path("log.txt");
         return logPath.string();
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

      std::string CProperties::logPath() const
      {
         return m_logPath;
      }

      const shared::CDataContainer& CProperties::configuration() const
      {
         return m_configuration;
      }
   }
} // namespace automation::script


