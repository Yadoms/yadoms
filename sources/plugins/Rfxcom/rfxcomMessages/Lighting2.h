#pragma once
#include "IRfxcomMessage.h"
#include "RFXtrxHelpers.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ILighting2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lightning2 protocol support
   //--------------------------------------------------------------
   class CLighting2 : public IRfxcomMessage
   {
   public:
      CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                 boost::shared_ptr<const yApi::IDeviceCommand> command,
                 const boost::shared_ptr<shared::CDataContainer>& deviceDetails);

      CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                 unsigned int subType,
                 const std::string& name,
                 const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration);

      CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                 const RBUF& rbuf,
                 size_t rbufSize);

      virtual ~CLighting2();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      void filter() const override;
      void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() override;
      // [END] IRfxcomMessage implementation

   protected:
      //--------------------------------------------------------------
      /// \brief	Set and create the subtype
      /// \param[in] subType              Device subType
      /// \param[in] deviceType           Device subType
      /// \return                         Created sub type
      //--------------------------------------------------------------
      boost::shared_ptr<ILighting2Subtype> createSubType(unsigned char subType,
                                                         ILighting2Subtype::EDeviceType deviceType = ILighting2Subtype::kUnknown) const;

      //--------------------------------------------------------------
      /// \brief	                        Build the device infos
      //--------------------------------------------------------------
      static std::string buildDeviceName(unsigned char subType,
                                         unsigned char houseCode,
                                         unsigned int id,
                                         unsigned char unitCode);

      static boost::shared_ptr<shared::CDataContainer> buildDeviceDetails(unsigned char subType,
                                                       unsigned char houseCode,
                                                       unsigned int id,
                                                       unsigned char unitCode);

   private:
      //--------------------------------------------------------------
      /// \brief	The device sub-type
      //--------------------------------------------------------------
      unsigned char m_subType;

      //--------------------------------------------------------------
      /// \brief	The device house code
      //--------------------------------------------------------------
      unsigned char m_houseCode;

      //--------------------------------------------------------------
      /// \brief	The device id
      //--------------------------------------------------------------
      unsigned int m_id;

      //--------------------------------------------------------------
      /// \brief	The device unit code
      //--------------------------------------------------------------
      unsigned char m_unitCode;

      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	The device details
      //--------------------------------------------------------------
      boost::shared_ptr<shared::CDataContainer> m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The sub-type management
      //--------------------------------------------------------------
      boost::shared_ptr<ILighting2Subtype> m_subTypeManager;

      //--------------------------------------------------------------
      /// \brief	The signalPower (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages


