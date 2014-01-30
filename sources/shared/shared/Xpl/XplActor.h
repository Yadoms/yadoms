#pragma once

#include <shared/Export.h>

//--------------------------------------------------------------
/// \class Actor for Xpl management used for source and target fields
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CXplActor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CXplActor();

   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param [in] actor : Actor to copy
   //--------------------------------------------------------------
   CXplActor(const CXplActor & actor);

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CXplActor();
   
   //--------------------------------------------------------------
   /// \brief	    Get the vendor Id
   /// \return     The vendor Id delivered by Xpl Project
   //--------------------------------------------------------------
   std::string getVendorId() const;

   //--------------------------------------------------------------
   /// \brief	    Set the vendor Id
   /// \param [in] vendorId : vendor Id delivered by Xpl Project
   //--------------------------------------------------------------
   void setVendorId(const std::string & vendorId);

   //--------------------------------------------------------------
   /// \brief	    Get the device Id
   /// \return     The device Id delivered by Xpl Project
   //--------------------------------------------------------------
   std::string getDeviceId() const;
   
   //--------------------------------------------------------------
   /// \brief	    Set the device Id
   /// \param [in] deviceId : device Id delivered by Xpl Project
   //--------------------------------------------------------------
   void setDeviceId(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Get the instance Id
   /// \return     The instance Id
   //--------------------------------------------------------------
   std::string getInstanceId() const;

   //--------------------------------------------------------------
   /// \brief	    Set the instance Id
   /// \param [in] instanceId : instance Id
   //--------------------------------------------------------------
   void setInstanceId(const std::string & instanceId);

   //--------------------------------------------------------------
   /// \brief	    Get the actor as it has to be in the XplMessage
   /// \return     the actor as it has to be in the XplMessage
   //--------------------------------------------------------------
   std::string toString() const;

   //--------------------------------------------------------------
   /// \brief	    Create a broadcast actor
   /// \return     the new broadcast actor
   //--------------------------------------------------------------
   static const CXplActor createBroadcastActor();

   //--------------------------------------------------------------
   /// \brief	    Create a new actor
   /// \param [in]   vendorId : The vendor Id delivered by Xpl Project
   /// \param [in]   deviceId : The device Id delivered by Xpl Project
   /// \param [in]   instanceId : The instance Id
   /// \return     the new actor
   //--------------------------------------------------------------
   static const CXplActor createActor(const std::string & vendorId, const std::string & deviceId, const std::string & instanceId);

      //--------------------------------------------------------------
   /// \brief	    Create a new actor
   /// \param [in]   rawActorString : Chaine de texte à parser pour representer un acteur
   /// \return     the new actor
   //--------------------------------------------------------------
   static const CXplActor parse(const std::string & rawActorString);

private:

   std::string m_vendorId;
   std::string m_deviceId;
   std::string m_instanceId;

   bool m_broadcastActive;

   static const int VendorIdMaxLength = 8;
   static const int DeviceIdMaxLength = 8;
   static const int InstanceIdMaxLength = 16;
};