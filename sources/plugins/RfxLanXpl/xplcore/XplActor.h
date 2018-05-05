#pragma once

namespace xplcore
{
   //--------------------------------------------------------------
   /// \brief Actor for Xpl management used for source and target fields
   //--------------------------------------------------------------
   class CXplActor
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
      CXplActor(const CXplActor& actor);
      CXplActor& operator=(const CXplActor& rhs);

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
      void setVendorId(const std::string& vendorId);

      //--------------------------------------------------------------
      /// \brief	    Get the device Id
      /// \return     The device Id delivered by Xpl Project
      //--------------------------------------------------------------
      std::string getDeviceId() const;

      //--------------------------------------------------------------
      /// \brief	    Set the device Id
      /// \param [in] deviceId : device Id delivered by Xpl Project
      //--------------------------------------------------------------
      void setDeviceId(const std::string& deviceId);

      //--------------------------------------------------------------
      /// \brief	    Get the instance Id
      /// \return     The instance Id
      //--------------------------------------------------------------
      std::string getInstanceId() const;

      //--------------------------------------------------------------
      /// \brief	    Set the instance Id
      /// \param [in] instanceId : instance Id
      //--------------------------------------------------------------
      void setInstanceId(const std::string& instanceId);

      //--------------------------------------------------------------
      /// \brief	    Get the actor as it has to be in the XplMessage
      /// \return     the actor as it has to be in the XplMessage
      //--------------------------------------------------------------
      std::string toString() const;

      //--------------------------------------------------------------
      /// \brief	    Create a broadcast actor
      /// \return     the new broadcast actor
      //--------------------------------------------------------------
      static CXplActor createBroadcastActor();

      //--------------------------------------------------------------
      /// \brief	    Create a new actor
      /// \param [in]   vendorId : The vendor Id delivered by Xpl Project
      /// \param [in]   deviceId : The device Id delivered by Xpl Project
      /// \param [in]   instanceId : The instance Id
      /// \return     the new actor
      //--------------------------------------------------------------
      static CXplActor createActor(const std::string& vendorId,
                                   const std::string& deviceId,
                                   const std::string& instanceId);

      //--------------------------------------------------------------
      /// \brief	    Create a new actor
      /// \param [in]   deviceId : The device Id delivered by Xpl Project
      /// \param [in]   instanceId : The instance Id
      /// \return     the new actor
      /// \note      When using this constructor, vendor Id used will be CXplConstants::getYadomsVendorId
      //--------------------------------------------------------------
      static CXplActor createActor(const std::string& deviceId,
                                   const std::string& instanceId);

      //--------------------------------------------------------------
      /// \brief	    Create a new actor
      /// \param [in]   rawActorString : Text string representing an actor
      /// \return     the new actor
      //--------------------------------------------------------------
      static CXplActor parse(const std::string& rawActorString);

      //--------------------------------------------------------------
      /// \brief	    Tells if the current instance is a broadcast one
      /// \return     true if the current instance is a broadcast one
      //--------------------------------------------------------------
      bool isBroadcastActor() const
      { return m_broadcastActive; }

      //--------------------------------------------------------------
      /// \brief	    Compare this instance with another. It compare vendorId, deviceId and instanceId.
      /// \param [in] rhs : instance to compare with
      /// \return     true if the two instances are equals
      //--------------------------------------------------------------
      bool operator==(const CXplActor& rhs) const;
   private:

      std::string m_vendorId;
      std::string m_deviceId;
      std::string m_instanceId;

      bool m_broadcastActive;
   };
} // namespace xplcore


