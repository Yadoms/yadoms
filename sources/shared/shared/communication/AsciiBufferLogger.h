#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	This class manage an ASCII buffer logger
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CAsciiBufferLogger : public IBufferLogger
   {  
   public:
      //--------------------------------------------------------------
      /// \brief	            Constructor
      //--------------------------------------------------------------
      CAsciiBufferLogger();

      //--------------------------------------------------------------
      /// \brief	            Destructor
      //--------------------------------------------------------------
      virtual ~CAsciiBufferLogger();

      // IBufferLogger implementation
      virtual void logReceived(const CByteBuffer& data);
      virtual void logSent(const CByteBuffer& data);
      virtual std::string msgToString(const CByteBuffer& data) const;
      // [END] IBufferLogger implementation
   };

} } // namespace shared::communication
