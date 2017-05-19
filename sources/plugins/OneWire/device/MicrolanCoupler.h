#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ISingleSwitch.h"

namespace device
{
   //--------------------------------------------------------------
   /// \brief	Microlan Coupler (Family 1F)
   //--------------------------------------------------------------
   class CMicrolanCoupler : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CMicrolanCoupler(EOneWireFamily family,
                       const std::string& id,
                       boost::shared_ptr<ioInterfaces::ISingleSwitch> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CMicrolanCoupler();

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
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

      //--------------------------------------------------------------
      /// \brief	The I/O access object
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::ISingleSwitch> m_io;

      //--------------------------------------------------------------
      /// \brief	The keyword associated with state
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CSwitch> m_state;

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device
