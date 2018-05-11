#pragma once

#include "INitramMessage.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "specificHistorizer/UPSStatus.h"

namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   //--------------------------------------------------------------
   /// \brief	The Blinds1 protocol support
   //--------------------------------------------------------------
   class CSummaryStatus : public INitramMessage
   {
   public:

      CSummaryStatus(std::string deviceName);

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] api                  Yadoms APi context
      /// \param[in] command              The command
      /// \param[in] deviceDetails        The device parameters
      /// \throw                          shared::exception::CInvalidParameter if fail to interpret command
      /// \note                           Use this constructor for command (to build RFXCom message)
      //--------------------------------------------------------------
      void decode(boost::shared_ptr<yApi::IYPluginApi> api,
                  const shared::communication::CByteBuffer& buffer);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSummaryStatus();

      // INitramMessage implementation
      boost::shared_ptr<shared::communication::CByteBuffer> encode() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) override;
      bool onlyAck() const override;
      // [END] INitramMessage implementation

      bool getACPresent();

   protected:

   private:

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	If the AC power supply is present
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_ACPresent;

      //--------------------------------------------------------------
      /// \brief	The UPS Status
      //--------------------------------------------------------------
      boost::shared_ptr<nitram::specificHistorizers::CStatus> m_UPSStatus;

      //--------------------------------------------------------------
      /// \brief	Low Runtime information
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_lowRuntime;

      //--------------------------------------------------------------
      /// \brief	Low Capcity Information
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_lowCapacity;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace nitramMessages