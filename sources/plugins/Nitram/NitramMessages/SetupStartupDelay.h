#pragma once
#include "INitramMessage.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime protocol support
   //--------------------------------------------------------------
   class CSetupStartupDelay : public INitramMessage
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] rbuf                 The received buffer
      /// \param[in] rbufSize             Message size, received from Rfxcom
      /// \note                           Use this constructor for received messages (to historize received data to Yadoms)
      /// \throw                          shared::exception::CInvalidParameter
      //--------------------------------------------------------------
      CSetupStartupDelay();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSetupStartupDelay();

      // INitramMessage implementation
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer) override;
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

      void setDelay(boost::posix_time::time_duration delay);

      void setMessageForShutdown();

   protected:

   private:

      //--------------------------------------------------------------
      /// \brief	The shutdown delay
      //--------------------------------------------------------------
      unsigned int m_delay;

   };
} // namespace nitramMessages


