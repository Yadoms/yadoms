#pragma once
#include "IIdentification.h"


namespace device {

   //--------------------------------------------------------------
   /// \brief	1-Wire identification
   //--------------------------------------------------------------
   class CIdentification : public IIdentification
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device Id
      /// \param[in]	model Device Model (chip reference)
      //--------------------------------------------------------------
      CIdentification(EOneWireFamily family, const std::string& id, const std::string& model);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CIdentification();

   protected:
      // IDevice implementation
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context) const;
      virtual EOneWireFamily family() const;
      virtual std::string id() const;
      virtual const std::string& deviceName() const;
      virtual const std::string& model() const;
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	Device family
      //--------------------------------------------------------------
      const EOneWireFamily m_family;

      //--------------------------------------------------------------
      /// \brief	Device Id
      //--------------------------------------------------------------
      const std::string m_id;

      //--------------------------------------------------------------
      /// \brief	Device Name
      //--------------------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief	Device Model (chip reference)
      //--------------------------------------------------------------
      const std::string m_model;
   };

} // namespace device