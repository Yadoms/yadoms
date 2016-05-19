#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ISecurity1Subtype.h"
#include "RFXtrx.h"

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
      enum
      {
         rfxValueKD101 = sTypeKD101,
         rfxValueSA30 = sTypeSA30
      };

      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      //--------------------------------------------------------------
      explicit CSecurity1KD101_SA30(const std::string& model);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSecurity1KD101_SA30();

      // ISecurity1Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& keyword, const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(unsigned char statusByte) override;
      unsigned char toProtocolState() const override;
      // [END] ISecurity1Subtype implementation

   private:
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
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


