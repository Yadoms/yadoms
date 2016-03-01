#pragma once
#include "ioInterfaces/ITemperatureIo.h"
#include "ioInterfaces/IMultiSwitch.h"
#include "ioInterfaces/ITemperature.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS temperature device reader
   //--------------------------------------------------------------
   class CTemperatureIo : public ioInterfaces::ITemperatureIo
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      explicit CTemperatureIo(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CTemperatureIo();

   protected:
      // ioInterfaces::ITemperatureIo implementation
      virtual bool readIo(unsigned char unit) const;
      virtual void writeIo(unsigned char unit, bool state) const;
      virtual double readTemperature() const;
      // [END] ioInterfaces::ITemperatureIo implementation

   private:
      //--------------------------------------------------------------
      /// \brief	Use standard IO accessors
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::IMultiSwitch> m_binaryIos;
      boost::shared_ptr<ioInterfaces::ITemperature> m_temperatureIo;
   };

} } // namespace owfs::io