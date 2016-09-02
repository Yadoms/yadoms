#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared {  namespace versioning {

   //-----------------------------------------------------
   ///\brief    Enum for plugin release state
   ///<         - kStable : plugin is stable
   ///<         - kReleaseCandidate : plugin is candidate for stable release
   ///>         - kBeta : plugin is in beta state
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EReleaseType, YADOMS_SHARED_EXPORT,
      ((Stable)(0))
      ((ReleaseCandidate)(1))
      ((Beta)(2))
   )

}} // namespace shared::versioning