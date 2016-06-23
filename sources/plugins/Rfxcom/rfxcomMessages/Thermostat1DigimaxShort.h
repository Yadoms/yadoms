#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IThermostat1Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat1 Digimax short keyword
   //--------------------------------------------------------------
   class CThermostat1DigimaxShort : public IThermostat1Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CThermostat1DigimaxShort();
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermostat1DigimaxShort();

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
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_currentTemperature;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace rfxcomMessages


