#pragma once
#include "IIdentification.h"

namespace device
{
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
   CIdentification(EOneWireFamily family,
                   const std::string &id,
                   const std::string &model);

   virtual ~CIdentification() = default;

protected:
   // IDevice implementation
   EOneWireFamily family() const override;
   std::string id() const override;
   const std::string &deviceName() const override;
   const std::string &model() const override;
   const boost::shared_ptr<shared::CDataContainer> &details() const override;
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

   //--------------------------------------------------------------
   /// \brief	Details
   //--------------------------------------------------------------
   const boost::shared_ptr<shared::CDataContainer> m_details;
};

} // namespace device