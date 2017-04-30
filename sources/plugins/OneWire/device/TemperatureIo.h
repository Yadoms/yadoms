#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ITemperatureIo.h"

namespace device
{
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
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CTemperatureIo(EOneWireFamily family,
                     const std::string& id,
                     boost::shared_ptr<ioInterfaces::ITemperatureIo> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CTemperatureIo();

   protected:
      // IDevice implementation
      boost::shared_ptr<const IIdentification> ident() const override
      {
         return m_identification;
      }
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override
      {
         return *m_keywords;
      }
      void setConfiguration(const shared::CDataContainer& configuration) override;
      void read() const override;
      void write(const std::string& keyword, const std::string& command) override;
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

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
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsWithTemperature;
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsWithoutTemperature;
      mutable std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >* m_keywords;
   };

} // namespace device
