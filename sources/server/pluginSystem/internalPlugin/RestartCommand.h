#pragma once

#include <shared/plugin/yadomsApi/historization/HistorizableType.hpp>
#include <shared/enumeration/EnumHelpers.hpp>

namespace pluginSystem {   namespace internalPlugin {


      //-----------------------------------------------------
      ///\brief               The restart command values
      //-----------------------------------------------------   
      DECLARE_ENUM_HEADER(ERestartCommand,
         ((RestartSystem)(0))
      );

      //-----------------------------------------------------
      ///\brief A shutdown command historizable object
      //-----------------------------------------------------
      class CRestartCommand : public shared::plugin::yadomsApi::historization::CHistorizableType<ERestartCommand>
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in] keywordName     Yadoms keyword name
         //-----------------------------------------------------
         CRestartCommand(const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CRestartCommand();

      };

   }
} // namespace pluginSystem::internalPlugin


