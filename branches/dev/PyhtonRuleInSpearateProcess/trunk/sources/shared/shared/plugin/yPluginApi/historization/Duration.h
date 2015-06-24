#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include <boost/date_time.hpp>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A duration (s) historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CDuration : public CSingleHistorizableData <boost::posix_time::time_duration>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] accessMode      Access mode
      //-----------------------------------------------------
      CDuration(const std::string& keywordName, const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDuration();
   };

} } } } // namespace shared::plugin::yPluginApi::historization
