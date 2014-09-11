#pragma once

#include "FrameBase.h"
#include <shared/exception/OutOfRange.hpp>


namespace web { namespace ws {

   class CFrameFactory
   {
   public:
      static boost::shared_ptr<CFrameBase> tryParse(const std::string & frameAsString);
   };

} //namespace ws
} //namespace web