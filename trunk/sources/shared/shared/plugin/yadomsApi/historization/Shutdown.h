#pragma once
#include <shared/Export.h>
#include "HistorizableType.hpp"
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi { namespace historization {


      //-----------------------------------------------------
      ///\brief               The shutdown command values
      //-----------------------------------------------------   
		DECLARE_ENUM_HEADER_SHARED(EShutdownCommand, YADOMS_SHARED_EXPORT,
         ((Shutdown)(0))
      );

      //-----------------------------------------------------
      ///\brief A shutdown command historizable object
      //-----------------------------------------------------
		class YADOMS_SHARED_EXPORT CShutdown : public CHistorizableType<EShutdownCommand>
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in] keywordName     Yadoms keyword name
         //-----------------------------------------------------
         CShutdown(const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CShutdown();

      };

 } } } } //namespace shared::plugin::yadomsApi::historization
