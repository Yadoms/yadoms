#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CBufferLogger : public IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         /// \param[in] os       Log target
         //--------------------------------------------------------------
         explicit CBufferLogger(std::ostream& os);

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CBufferLogger();

         // IBufferLogger implementation
         void logReceived(const CByteBuffer& data) override;
         void logSent(const CByteBuffer& data) override;
         // [END] IBufferLogger implementation

      protected:
         std::string msgToString(const CByteBuffer& data) const;

      private:
         std::ostream& m_os;
      };
   }
} // namespace shared::communication


