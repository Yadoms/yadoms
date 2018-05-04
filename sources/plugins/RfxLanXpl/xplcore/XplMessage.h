#pragma once

#include "XplMessageSchemaIdentifier.h"
#include "XplActor.h"

namespace xplcore
{
   //--------------------------------------------------------------
   /// \class It represent a xpl message
   //--------------------------------------------------------------
   class CXplMessage
   {
   public:
      enum ETypeIdentifier
      {
         kXplCommand,
         kXplStat,
         kXplTrigger
      };

      //--------------------------------------------------------------
      /// \brief	    Constructor
      //--------------------------------------------------------------
      CXplMessage();

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param [in] typeId : Type identifier (cmd, trig or stat)
      /// \param [in] source : source actor
      /// \param [in] target : target actor
      /// \param [in] messageSchemaIdentifier : message Identifier
      //--------------------------------------------------------------
      CXplMessage(const ETypeIdentifier& typeId,
                  const CXplActor& source,
                  const CXplActor& target,
                  const CXplMessageSchemaIdentifier& messageSchemaIdentifier);

      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param [in] message : message to copy
      //--------------------------------------------------------------
      CXplMessage(const CXplMessage& message);
      CXplMessage& operator=(const CXplMessage&) = delete;

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CXplMessage();

      //--------------------------------------------------------------
      /// \brief	    Set the Type identifier (cmd, trig or stat)
      /// \param [in] typeId : type identifier of the message
      //--------------------------------------------------------------
      void setTypeIdentifier(const ETypeIdentifier& typeId);

      //--------------------------------------------------------------
      /// \brief	    Get the type identifier
      /// \return     The type identifier of the message
      //--------------------------------------------------------------
      ETypeIdentifier getTypeIdentifier() const;

      //--------------------------------------------------------------
      /// \brief	    Get the type identifier as a string
      /// \return     The type identifier of the message
      //--------------------------------------------------------------
      std::string getTypeIdentifierAsString() const;

      //--------------------------------------------------------------
      /// \brief	    Set the source
      /// \param [in] source : source of the message
      //--------------------------------------------------------------
      void setSource(const CXplActor& source);

      //--------------------------------------------------------------
      /// \brief	    Get the source
      /// \return     The source of the message
      //--------------------------------------------------------------
      CXplActor getSource() const;

      //--------------------------------------------------------------
      /// \brief	    Set the target
      /// \param [in] target : target of the message
      //--------------------------------------------------------------
      void setTarget(const CXplActor& target);

      //--------------------------------------------------------------
      /// \brief	    Get the target
      /// \return     The target of the message
      //--------------------------------------------------------------
      CXplActor getTarget() const;

      //--------------------------------------------------------------
      /// \brief	    Increment the hop count
      //--------------------------------------------------------------
      void incrementHop();

      //--------------------------------------------------------------
      /// \brief	    Get the Hop
      /// \return     the Hop count
      //--------------------------------------------------------------
      int getHop() const;

      //--------------------------------------------------------------
      /// \brief	    Set the Hop
      /// \param [in] hop : value of the hop to set
      //--------------------------------------------------------------
      void setHop(int hop);

      //--------------------------------------------------------------
      /// \brief	    Set the message Schema Identifier
      /// \param [in] messageSchemaIdentifier : message schema identifier of the message
      //--------------------------------------------------------------
      void setMessageSchemaIdentifier(const CXplMessageSchemaIdentifier& messageSchemaIdentifier);

      //--------------------------------------------------------------
      /// \brief	    Get the Message Schema Identifier
      /// \return     The message schema identifier
      //--------------------------------------------------------------
      const CXplMessageSchemaIdentifier& getMessageSchemaIdentifier() const;

      //--------------------------------------------------------------
      /// \brief	    Permit to add a key/value element to the body
      /// \param [in] name : name of the key/value element
      /// \param [in] value : value of the key/value element
      //--------------------------------------------------------------
      void addToBody(const std::string& name, const std::string& value);

      //--------------------------------------------------------------
      /// \brief	    Get the Map of the body
      /// \return     The Map containing all the body
      //--------------------------------------------------------------
      const std::map<std::string, std::string>& getBody() const;

      //--------------------------------------------------------------
      /// \brief	    Check if body contains the given value
      /// \param [in] key : key of the key/value element
      /// \return     true if body contains the value
      //--------------------------------------------------------------
      bool hasBodyValue(const std::string& key) const;

      //--------------------------------------------------------------
      /// \brief	    Get body value
      /// \param [in] key : key of the key/value element
      /// \return     A value contained in the body
      /// \throw      CXplException if key was not found
      //--------------------------------------------------------------
      const std::string& getBodyValue(const std::string& key) const;

      //--------------------------------------------------------------
      /// \brief	    Get body value, with default value
      /// \param [in] key : key of the key/value element
      /// \param [in] defaultValue : default value, returned if key was not found
      /// \return     A value contained in the body, or the default value if key was not found
      //--------------------------------------------------------------
      const std::string& getBodyValue(const std::string& key,
                                      const std::string& defaultValue) const;

      //--------------------------------------------------------------
      /// \brief	    Get the message as it has to be sent
      /// \return     the message as it has to be sent
      //--------------------------------------------------------------
      std::string toString() const;

      //--------------------------------------------------------------
      /// \brief	    Parse A string and build an XplMessage With
      /// \param [in] rawMessage : message to Parse
      //--------------------------------------------------------------
      static CXplMessage parse(const std::string& rawMessage);

      //--------------------------------------------------------------
      /// \brief	    Create a heartbeat request message
      /// \param [in] source : the source
      //--------------------------------------------------------------
      static CXplMessage createHeartbeatRequest(const CXplActor& source);

   private:
      ETypeIdentifier m_typeIdentifier;
      int m_hop;
      CXplActor m_source;
      CXplActor m_target;
      CXplMessageSchemaIdentifier m_messageSchemaIdentifier;

      std::map<std::string, std::string> m_body;

      static const size_t MaxMessageSize = 1500;

      static const std::string XplCmdTypeIdentifier;
      static const std::string XplStatTypeIdentifier;
      static const std::string XplTrigTypeIdentifier;

      static const std::string XplHopHeader;
      static const std::string XplSourceHeader;
      static const std::string XplTargetHeader;
   };
} // namespace xplcore


