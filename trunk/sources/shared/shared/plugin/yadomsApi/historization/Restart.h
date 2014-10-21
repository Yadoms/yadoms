#pragma once
#include <shared/Export.h>
#include "SingleHistorizableData.hpp"
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization {

   
      //-----------------------------------------------------
      ///\brief               The restart command values
      //-----------------------------------------------------   
		DECLARE_ENUM_HEADER_SHARED(ERestartCommand, YADOMS_SHARED_EXPORT,
         ((Restart)(0))
      );

      //-----------------------------------------------------
      ///\brief A shutdown command historizable object
      //-----------------------------------------------------
		class YADOMS_SHARED_EXPORT CRestart : public CSingleHistorizableData<ERestartCommand>
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in] keywordName     Yadoms keyword name
         //-----------------------------------------------------
         CRestart(const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CRestart();

      };

 } } } } //namespace shared::plugin::yadomsApi::historization

