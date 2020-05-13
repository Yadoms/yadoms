#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IChimeSubtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Chime Byron SX keyword
   //--------------------------------------------------------------
   class CChimeByronMp001 : public IChimeSubtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      explicit CChimeByronMp001(const std::string& model);

      // ILighting2Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(const std::string& yadomsCommand, const boost::shared_ptr<shared::CDataContainer>& deviceDetails) override;
      void reset() override;
      unsigned int idFromProtocol(unsigned char id1,
                                  unsigned char id2,
                                  unsigned char sound) override;
      void idToProtocol(unsigned int id,
                        unsigned char& id1,
                        unsigned char& id2,
                        unsigned char& sound) const override;
      void setFromProtocolState(unsigned char cmd) override;
      void toProtocolState(unsigned char& sound) const override;
      // [END] ILighting2Subtype implementation

   private:
      const std::string m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEvent> m_event;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages
