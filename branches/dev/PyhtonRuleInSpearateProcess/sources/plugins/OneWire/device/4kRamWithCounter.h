#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IMultiCounter.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	4Ko RAM with counters (Family 1D)
   //--------------------------------------------------------------
   class C4kRamWithCounter : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	context yApi context
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      C4kRamWithCounter(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IMultiCounter> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~C4kRamWithCounter();

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
      boost::shared_ptr<ioInterfaces::IMultiCounter> m_io;

      //--------------------------------------------------------------
      /// \brief	The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CCounter> m_countA;
      boost::shared_ptr<yApi::historization::CCounter> m_countB;
      std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device