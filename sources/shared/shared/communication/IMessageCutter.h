#pragma once
#include <shared/Export.h>
#include "IMessagePart.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This interface manage cutting too long messages
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT IMessageCutter
      {
      public:
         virtual ~IMessageCutter()
         {
         }

         virtual boost::shared_ptr<std::vector<boost::shared_ptr<IMessagePart>>> cut(boost::shared_ptr<unsigned char[]> message,
                                                                                     size_t messageSize) const = 0;
      };
   }
} // namespace shared::communication


