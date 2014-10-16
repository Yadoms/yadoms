#pragma once

#include <shared/plugin/yadomsApi/historization/HistorizableType.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace pluginSystem {
   namespace internalPlugin {


      //-----------------------------------------------------
      ///\brief               The shutdown command values
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER(EShutdownCommand,
      ((ShutdowSystem)(0))
         );

      //-----------------------------------------------------
      ///\brief A shutdown command historizable object
      //-----------------------------------------------------
      class CShutdownCommand : public shared::plugin::yadomsApi::historization::CHistorizableType<EShutdownCommand>
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in] keywordName     Yadoms keyword name
         //-----------------------------------------------------
         CShutdownCommand(const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CShutdownCommand();

      };

   }
} // namespace pluginSystem::internalPlugin


