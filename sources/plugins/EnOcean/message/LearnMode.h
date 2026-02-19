#pragma once
#include <string>

#include "shared/Log.h"

namespace message
{
   enum class LearnMode
   {
      SimpleLearnMode,
      AdvancedLearnMode,
      AdvancedLearnModeSelectRepeater,
   };

   static std::string ToString(LearnMode learnMode)
   {
      switch (learnMode)
      {
      case LearnMode::SimpleLearnMode:
         return "Simple";
      case LearnMode::AdvancedLearnMode:
         return "Advanced";
      case LearnMode::AdvancedLearnModeSelectRepeater:
         return "Advanced, select repeater";
      default: // NOLINT(clang-diagnostic-covered-switch-default)
         return "Unknown";
      }
   }

   static unsigned char ToProtocolValue(const LearnMode learnMode)
   {
      switch (learnMode)
      {
      case LearnMode::SimpleLearnMode:
         return 0;
      case LearnMode::AdvancedLearnMode:
         return 1;
      case LearnMode::AdvancedLearnModeSelectRepeater:
         return 2;
      default: // NOLINT(clang-diagnostic-covered-switch-default)
         throw std::invalid_argument("LeanMode invalid value"); // NOLINT(clang-diagnostic-covered-switch-default)
      }
   }

   static LearnMode ToLearnMode(const unsigned char protocolValue)
   {
      switch (protocolValue)
      {
      case 0:
         return LearnMode::SimpleLearnMode;
      case 1:
         return LearnMode::AdvancedLearnMode;
      case 2:
         return LearnMode::AdvancedLearnModeSelectRepeater;
      default:
         YADOMS_LOG(error) << "Smart Ack response : unsupported LearnModeExtended value " << static_cast<int>(protocolValue);
         return LearnMode::SimpleLearnMode;
      }
   }
} // namespace message
