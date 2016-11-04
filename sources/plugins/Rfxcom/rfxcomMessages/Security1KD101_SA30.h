#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity1Subtype.h"
#include "RFXtrxHelpers.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Scurity1 KD101 subtype
   //--------------------------------------------------------------
   class CSecurity1KD101_SA30 : public ISecurity1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        The RFXCom subtype value
      //--------------------------------------------------------------
      enum SubType
      {
         rfxValueKD101 = sTypeKD101,
         rfxValueSA30 = sTypeSA30,
         rfxValueRM174RF = sTypeRM174RF
      };

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] subtype              The device subtype
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      CSecurity1KD101_SA30(SubType subtype,
                           const std::string& model);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity1KD101_SA30();

      // ISecurity1Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char statusByte) override;
      unsigned char toProtocolState() const override;
      unsigned long idFromProtocol(const RBUF& rbuf) const override;
      // [END] ISecurity1Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The device subtype
      //--------------------------------------------------------------
      const SubType m_subtype;

      //--------------------------------------------------------------
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string m_model;
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_alarm;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
