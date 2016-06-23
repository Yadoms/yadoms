#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "ILighting5Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lighting5 On/off keyword
   //--------------------------------------------------------------
   class CLighting5MdRemote : public ILighting5Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CLighting5MdRemote();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting5MdRemote();

      // ILighting5Subtype implementation
      std::string getModel() const override;
      boost::shared_ptr<const yApi::historization::IHistorizable> keyword() const override;
      void set(const std::string& yadomsCommand) override;
      void reset() override;
      size_t getMessageNb() const override;
      void setFromProtocolState(unsigned char cmdByte,
                                unsigned char levelByte) override;
      void toProtocolState(size_t idxMessage,
                           unsigned char& cmdByte,
                           unsigned char& levelByte) const override;
      // [END] ILighting5Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CDimmable> m_keyword;
   };
} // namespace rfxcomMessages


