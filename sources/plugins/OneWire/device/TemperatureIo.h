#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ITemperatureIo.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	Temperature + IO device (Family 42)
   //--------------------------------------------------------------
   class CTemperatureIo : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	context yApi context
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CTemperatureIo(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperatureIo> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CTemperatureIo();

   protected:
      // IDevice implementation
      virtual boost::shared_ptr<const IIdentification> ident() const;
      virtual void declare();
      virtual void historize();
      virtual void set(const std::string& keyword, const std::string& command);
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

      //--------------------------------------------------------------
      /// \brief	The yApi context
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::IYPluginApi> m_context;

      //--------------------------------------------------------------
      /// \brief	The I/O access object
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::ITemperatureIo> m_io;

      //--------------------------------------------------------------
      /// \brief	The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_ioA;
      boost::shared_ptr<yApi::historization::CSwitch> m_ioB;
      boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   };

} // namespace device