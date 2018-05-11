#pragma once

#include "INitramMessage.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	Request Runtime information
   //--------------------------------------------------------------
   class CRuntime : public INitramMessage
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
      CRuntime(std::string deviceName);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CRuntime();

      // INitramMessage implementation
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer) override;
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

      void setRunTimeIndefined();

   protected:

   private:

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The food temperature (°C)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDuration> m_RuntimeDuration;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      mutable std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;

      boost::posix_time::time_duration m_RunTime;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      bool m_shouldBeHistorize;
   };
} // namespace nitramMessages


