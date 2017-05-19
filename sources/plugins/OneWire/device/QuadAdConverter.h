#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IQuadAdConverter.h"

namespace device
{
   //--------------------------------------------------------------
   /// \brief	4-AD converters (Family 20)
   //--------------------------------------------------------------
   class CQuadAdConverter : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CQuadAdConverter(EOneWireFamily family,
                       const std::string& id,
                       boost::shared_ptr<ioInterfaces::IQuadAdConverter> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CQuadAdConverter();

   protected:
      // IDevice implementation
      boost::shared_ptr<const IIdentification> ident() const override
      {
         return m_identification;
      }
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override
      {
         return m_keywords;
      }
      void setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                            const shared::CDataContainer& configuration) override;
      void read() const override;
      void write(const std::string& keyword, const std::string& command) override;
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The AD Converter number
      //--------------------------------------------------------------
      enum { NbAdConverter = 4 };

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
      boost::shared_ptr<ioInterfaces::IQuadAdConverter> m_io;

      //--------------------------------------------------------------
      /// \brief	The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CVoltage> m_keyword[NbAdConverter];
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };
} // namespace device
