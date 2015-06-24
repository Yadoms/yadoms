#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ITemperature.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	Common class for single temperature devices
   //--------------------------------------------------------------
   class CSingleTemperature : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	model Device model (chip reference)
      /// \param[in]	context yApi context
      /// \param[in]	io I/O access object
      /// \param[in]	expectedFamily Expected family (used for runtime check)
      //--------------------------------------------------------------
      CSingleTemperature(EOneWireFamily family, const std::string& id, const std::string& model, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperature> io, EOneWireFamily expectedFamily);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSingleTemperature();

      //--------------------------------------------------------------
      /// \brief	Check if read temperature is valid
      /// \param[in] temperature Temperature value to check
      /// \return true if temperature is valid
      //--------------------------------------------------------------
      static bool isTemperatureValid(double temperature);

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
      boost::shared_ptr<ioInterfaces::ITemperature> m_io;

      //--------------------------------------------------------------
      /// \brief	The temperature (°C)
      //--------------------------------------------------------------
      yApi::historization::CTemperature m_temperature;
   };

} // namespace device