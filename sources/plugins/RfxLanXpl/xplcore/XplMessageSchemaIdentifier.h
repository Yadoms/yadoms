#pragma once

namespace xplcore
{
   //--------------------------------------------------------------
   /// \brief Message Schema Identifier for Xpl message (x10.basic)
   //--------------------------------------------------------------
   class CXplMessageSchemaIdentifier
   {
   public:

      //--------------------------------------------------------------
      /// \brief	    Constructor
      //--------------------------------------------------------------
      CXplMessageSchemaIdentifier();

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param [in] classId : Class Id 
      /// \param [in] typeId : Type Id
      //--------------------------------------------------------------
      CXplMessageSchemaIdentifier(const std::string& classId,
                                  const std::string& typeId);

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param [in] identifier : Message Schema Identifier to copy
      //--------------------------------------------------------------
      CXplMessageSchemaIdentifier(const CXplMessageSchemaIdentifier& identifier);
      CXplMessageSchemaIdentifier& operator=(const CXplMessageSchemaIdentifier& rhs);

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CXplMessageSchemaIdentifier();

      //--------------------------------------------------------------
      /// \brief	    Set the class Id
      /// \param [in] classId : class Id delivered by Xpl Project
      //--------------------------------------------------------------
      void setClassId(const std::string& classId);

      //--------------------------------------------------------------
      /// \brief	    Get the class Id
      /// \return     The class Id delivered by Xpl Project
      //--------------------------------------------------------------
      std::string getClassId() const;

      //--------------------------------------------------------------
      /// \brief	    Set the type Id
      /// \param [in] typeId : type Id delivered by Xpl Project
      //--------------------------------------------------------------
      void setTypeId(const std::string& typeId);

      //--------------------------------------------------------------
      /// \brief	    Get the type Id
      /// \return     The type Id delivered by Xpl Project
      //--------------------------------------------------------------
      std::string getTypeId() const;

      //--------------------------------------------------------------
      /// \brief	    Get the Message Schema Identifier as it has to be in the XplMessage
      /// \return     the Message Schema Identifier as it has to be in the XplMessage
      //--------------------------------------------------------------
      std::string toString() const;

      //--------------------------------------------------------------
      /// \brief	    Get the Message Schema Identifier from a string
      /// \return     the Message Schema Identifier
      //--------------------------------------------------------------
      static CXplMessageSchemaIdentifier parse(const std::string& rawMessageSchemaIdentifier);

      //--------------------------------------------------------------
      /// \brief	    Create a Message Schema Identifier for heartbeat.app
      /// \return     the Message Schema Identifier 
      //--------------------------------------------------------------
      static CXplMessageSchemaIdentifier createHeartbeatAppMessageSchemaIdentifer();
      //--------------------------------------------------------------
      /// \brief	    Create a Message Schema Identifier for heartbeat.request
      /// \return     the Message Schema Identifier 
      //--------------------------------------------------------------
      static CXplMessageSchemaIdentifier createHeartbeatRequestMessageSchemaIdentifer();

      //--------------------------------------------------------------
      /// \brief	    Permit to know if a Message Schema Identifier is an heartbeat
      /// \return     the Message Schema Identifier 
      //--------------------------------------------------------------
      static bool isHeartbeatMessageSchemaIdentifier(const CXplMessageSchemaIdentifier& msi);

      //--------------------------------------------------------------
      /// \brief	    Permit to know if a Message Schema Identifier is an heartbeat.request
      /// \return     true if heartbeat.request, false in other cases
      //--------------------------------------------------------------
      static bool isHeartbeatRequest(const CXplMessageSchemaIdentifier& msi);

      //--------------------------------------------------------------
      /// \brief	    Permit to know if a Message Schema Identifier is an heartbeat.app
      /// \return     true if heartbeat.app, false in other cases
      //--------------------------------------------------------------
      static bool isHeartbeatApp(const CXplMessageSchemaIdentifier& msi);

   private:
      std::string m_classId;
      std::string m_typeId;
   };
} // namespace xplcore


