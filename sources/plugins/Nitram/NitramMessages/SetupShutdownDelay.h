#pragma once

#include "INitramMessage.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	The Camera1 protocol support
   //--------------------------------------------------------------
   class CSetupShutdownDelay : public INitramMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      CSetupShutdownDelay();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSetupShutdownDelay();

      // INitramMessage implementation
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer) override;
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

      void setDelay(boost::posix_time::time_duration delay);

   protected:

   private:

      //--------------------------------------------------------------
      /// \brief	The shutdown delay
      //--------------------------------------------------------------
      unsigned int m_delay;

   };
} // namespace nitramMessages


