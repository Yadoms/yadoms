#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ILighting2Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lighting2 dimmable keyword
   //--------------------------------------------------------------
   class CLighting2OnOffOrDimmable : public ILighting2Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] model                The device model
      /// \param[in] deviceType           The device type
      //--------------------------------------------------------------
      explicit CLighting2OnOffOrDimmable(const std::string& model,
                                         EDeviceType deviceType);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting2OnOffOrDimmable();

      // ILighting2Subtype implementation
      std::string getModel() const override;
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const override;
      void set(boost::shared_ptr<const yApi::IDeviceCommand> yadomsCommand) override;
      void reset() override;
      void idFromProtocol(unsigned char id1Byte,
                          unsigned char id2Byte,
                          unsigned char id3Byte,
                          unsigned char id4Byte,
                          unsigned char& houseCode,
                          unsigned int& id) const override;
      void idToProtocol(unsigned char houseCode,
                        unsigned int id,
                        unsigned char& id1Byte,
                        unsigned char& id2Byte,
                        unsigned char& id3Byte,
                        unsigned char& id4Byte) const override;
      void setFromProtocolState(unsigned char cmdByte,
                                unsigned char levelByte) override;
      void toProtocolState(unsigned char& cmdByte,
                           unsigned char& levelByte) const override;
      // [END] ILighting2Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The device model
      //--------------------------------------------------------------
      const std::string m_model;

      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_state;
      boost::shared_ptr<yApi::historization::CDimmable> m_level;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;
   };
} // namespace rfxcomMessages


