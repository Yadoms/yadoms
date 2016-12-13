#pragma once
#include <shared/Export.h>
#include "IBufferLogger.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage an ASCII buffer logger
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CAsciiBufferLogger : public IBufferLogger
      {
      public:
         //--------------------------------------------------------------
         /// \brief	            Constructor
         /// \param[in] os       Log target
         //--------------------------------------------------------------
         explicit CAsciiBufferLogger(std::ostream& os);

         //--------------------------------------------------------------
         /// \brief	            Destructor
         //--------------------------------------------------------------
         virtual ~CAsciiBufferLogger();

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


