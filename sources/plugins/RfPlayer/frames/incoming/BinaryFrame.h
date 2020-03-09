#pragma once
#include <shared/communication/Buffer.hpp>
#include <shared/DataContainer.h>

#include <shared/plugin/yPluginApi/IYPluginApi.h>


namespace yApi = shared::plugin::yPluginApi;
namespace frames {
namespace incoming {

   class CBinaryFrame
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param[in] sourceDest  The sourceDest byte from header
      /// \param[in] content     Binary buffer
      //--------------------------------------------------------------
      CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content);

      //--------------------------------------------------------------
      /// \brief	               Destructor
      //--------------------------------------------------------------
      virtual ~CBinaryFrame();

      //--------------------------------------------------------------
      /// \brief	               The header size of binary frame
      //--------------------------------------------------------------
      static const int HeaderSize = 5;

      //--------------------------------------------------------------
      /// \brief	               Get the sourceDest of frame
      /// \return                The sourceDest byte of frame
      //--------------------------------------------------------------
      char getSourceDest() const;

      //--------------------------------------------------------------
      /// \brief	               Get the binary content of frame
      /// \return                The binary content of frame
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > getContent() const;

      //--------------------------------------------------------------
      /// \brief	               Print the binary frame to log
      /// \param [in] out        The log stream to write
      //--------------------------------------------------------------
      void printToLog(std::ostream & out) const;

      //--------------------------------------------------------------
      /// \brief	               Historize data to pluginAPI
      /// \param [in] api        The pluginAPI
      //--------------------------------------------------------------
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api);

   private:
      //--------------------------------------------------------------
      /// \brief	               Identify oregon device from its idPhy and channel
      /// \param [in] idPhy      The id from frame
      /// \param [in] channel    The channel
      //--------------------------------------------------------------
      void identifyOregonDeviceFromIdPhy(unsigned short idPhy, unsigned short channel);

      //--------------------------------------------------------------
      /// \brief	               Manage the battery level 
      /// \param [in] qualifier  The qualifier from frame
      //--------------------------------------------------------------
      void manageOregonBatteryFromQualifier(unsigned short qualifier);

      //--------------------------------------------------------------
      /// \brief	               Find and identify device and keywords contained in frame
      //--------------------------------------------------------------
      void buildDeviceInfo();

      //--------------------------------------------------------------
      /// \brief	               The sourceDest of frame
      //--------------------------------------------------------------
      char m_sourceDest;
      //--------------------------------------------------------------
      /// \brief	               The binary frame content (without header)
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > m_content;

      //--------------------------------------------------------------
      /// \brief	               The device name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	               The device model
      //--------------------------------------------------------------
      std::string m_deviceModel;

      //--------------------------------------------------------------
      /// \brief	               The device details
      //--------------------------------------------------------------
      shared::CDataContainerSharedPtr m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} //namespace incoming
} //namespace frames

