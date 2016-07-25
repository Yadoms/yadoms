#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IThermostat1Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat1 Digimax keyword
   //--------------------------------------------------------------
   class CThermostat1Digimax : public IThermostat1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat1Digimax();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermostat1Digimax();

      // ILighting2Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override;
      void set(const std::string& yadomsCommand) override;
      void reset() override;
      void setFromProtocolState(const RBUF& thermostat1Rbuf) override;
      void toProtocolState(RBUF& thermostat1Rbuf) const override;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_currentTemperature;
      boost::shared_ptr<yApi::historization::CTemperature> m_setPoint;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


