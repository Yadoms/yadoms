#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IQuadAdConverter.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

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
      /// \param[in]	context yApi context
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CQuadAdConverter(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IQuadAdConverter> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CQuadAdConverter();

   protected:
      // IDevice implementation
      virtual boost::shared_ptr<const IIdentification> ident() const;
      virtual void declare();
      virtual void historize();
      virtual void set(const std::string& keyword, const std::string& command);
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
      std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device