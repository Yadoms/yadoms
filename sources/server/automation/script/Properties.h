#pragma once
#include "IProperties.h"
#include <database/entities/Entities.h>
#include "IPathProvider.h"

namespace automation
{
   namespace script
   {
      //-----------------------------------------------------
      ///\brief The script properties
      //-----------------------------------------------------
      class CProperties : public IProperties
      {
      public:
         //-----------------------------------------------------
         ///\brief                  Constructor
         ///\param[in] ruleData     Rule data
         //-----------------------------------------------------
         explicit CProperties(boost::shared_ptr<const database::entities::CRule> ruleData);

         //-----------------------------------------------------
         ///\brief               Destructor
         //-----------------------------------------------------
         virtual ~CProperties();

         // IProperties Implementation
         std::string interpreterName() const override;
         bool isModelBased() const override;
         boost::filesystem::path scriptPath() const override;
         const shared::CDataContainerSharedPtr& configuration() const override;
         // [END] IProperties Implementation

      protected:
         //-----------------------------------------------------
         ///\brief               Build the script Path from the rule data
         ///\param[in] ruleData Rule data
         ///\return              The script path
         //-----------------------------------------------------
         boost::filesystem::path buildScriptPath(boost::shared_ptr<const database::entities::CRule> ruleData) const;

      private:
         //-----------------------------------------------------
         ///\brief               The interpreter name
         //-----------------------------------------------------
         const std::string m_interpreterName;

         //-----------------------------------------------------
         ///\brief               Model-based or locally defined flag
         //-----------------------------------------------------
         const bool m_modelBased;

         //-----------------------------------------------------
         ///\brief               The script path
         //-----------------------------------------------------
         const boost::filesystem::path m_scriptPath;

         //-----------------------------------------------------
         ///\brief               The configuration
         //-----------------------------------------------------
         const shared::CDataContainerSharedPtr& m_configuration;
      };
   }
} // namespace automation::script
