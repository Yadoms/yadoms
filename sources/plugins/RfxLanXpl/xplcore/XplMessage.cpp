#include "stdafx.h"
#include "XplMessage.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplException.h"
#include <shared/StringExtension.h>

namespace xplcore
{
   const std::string CXplMessage::XplCmdTypeIdentifier = "xpl-cmnd";
   const std::string CXplMessage::XplStatTypeIdentifier = "xpl-stat";
   const std::string CXplMessage::XplTrigTypeIdentifier = "xpl-trig";

   const std::string CXplMessage::XplHopHeader = "hop";
   const std::string CXplMessage::XplSourceHeader = "source";
   const std::string CXplMessage::XplTargetHeader = "target";

   CXplMessage::CXplMessage()
   {
      m_hop = 1;
      setTypeIdentifier(kXplStat);
      setSource(CXplActor::createBroadcastActor());
      setTarget(CXplActor::createBroadcastActor());
      setMessageSchemaIdentifier(CXplMessageSchemaIdentifier("hbeat", "app"));
   }

   CXplMessage::CXplMessage(const ETypeIdentifier& typeId,
                            const CXplActor& source,
                            const CXplActor& target,
                            const CXplMessageSchemaIdentifier& messageSchemaIdentifier)
   {
      m_hop = 1;
      setTypeIdentifier(typeId);
      setSource(source);
      setTarget(target);
      setMessageSchemaIdentifier(messageSchemaIdentifier);
   }

   CXplMessage::CXplMessage(const CXplMessage& message)
   {
      m_body.insert(message.m_body.begin(), message.m_body.end()); //make the copy
      m_hop = message.getHop();
      setTypeIdentifier(message.getTypeIdentifier());
      setSource(message.getSource());
      setTarget(message.getTarget());
      setMessageSchemaIdentifier(message.m_messageSchemaIdentifier);
   }


   CXplMessage::~CXplMessage()
   {
   }

   void CXplMessage::setTypeIdentifier(const ETypeIdentifier& typeId)
   {
      m_typeIdentifier = typeId;
   }

   CXplMessage::ETypeIdentifier CXplMessage::getTypeIdentifier() const
   {
      return m_typeIdentifier;
   }

   std::string CXplMessage::getTypeIdentifierAsString() const
   {
      switch (m_typeIdentifier)
      {
      case kXplCommand:
         return XplCmdTypeIdentifier;
      case kXplStat:
         return XplStatTypeIdentifier;
      case kXplTrigger:
         return XplTrigTypeIdentifier;
      }
      throw CXplException("Unknown Type Identifier");
   }

   void CXplMessage::setSource(const CXplActor& source)
   {
      m_source = source;
   }

   CXplActor CXplMessage::getSource() const
   {
      return m_source;
   }

   void CXplMessage::setTarget(const CXplActor& target)
   {
      m_target = target;
   }

   CXplActor CXplMessage::getTarget() const
   {
      return m_target;
   }

   void CXplMessage::incrementHop()
   {
      m_hop++;
   }

   int CXplMessage::getHop() const
   {
      return m_hop;
   }

   void CXplMessage::setHop(int hop)
   {
      m_hop = hop;
   }

   void CXplMessage::setMessageSchemaIdentifier(const CXplMessageSchemaIdentifier& messageSchemaIdentifier)
   {
      m_messageSchemaIdentifier = CXplMessageSchemaIdentifier(messageSchemaIdentifier);
   }

   const CXplMessageSchemaIdentifier& CXplMessage::getMessageSchemaIdentifier() const
   {
      return m_messageSchemaIdentifier;
   }

   void CXplMessage::addToBody(const std::string& name,
                               const std::string& value)
   {
      CXplHelper::checkRules(CXplHelper::kBody, name);

      //the body element must not exist
      if (m_body.find(name) != m_body.end())
         throw CXplException("Element " + name + " can't be defined more than once");

      m_body[name] = value;
   }

   const std::map<std::string, std::string>& CXplMessage::getBody() const
   {
      return m_body;
   }

   bool CXplMessage::hasBodyValue(const std::string& key) const
   {
      std::map<std::string, std::string>::const_iterator it = m_body.find(key);
      return (it != m_body.end());
   }

   const std::string& CXplMessage::getBodyValue(const std::string& key) const
   {
      std::map<std::string, std::string>::const_iterator it = m_body.find(key);
      if (it == m_body.end())
         throw CXplException("XPL message body value not found : " + key);
      return it->second;
   }

   const std::string& CXplMessage::getBodyValue(const std::string& key,
                                                const std::string& defaultValue) const
   {
      auto it = m_body.find(key);
      if (it == m_body.end())
         return defaultValue;
      return it->second;
   }

   std::string CXplMessage::toString() const
   {
      std::stringstream ss;

      auto lineFeed = '\n';

      ss << getTypeIdentifierAsString();

      ss << lineFeed;

      //Header block
      ss << "{" << lineFeed;
      ss << XplHopHeader << "=" << m_hop << lineFeed;
      ss << XplSourceHeader << "=" << m_source.toString() << lineFeed;
      ss << XplTargetHeader << "=" << m_target.toString() << lineFeed;
      ss << "}" << lineFeed;

      //Message Schema
      ss << m_messageSchemaIdentifier.toString() << lineFeed;

      //Message body
      ss << "{" << lineFeed;
      //we browse the map
      for (auto i = m_body.begin(); i != m_body.end(); ++i)
      {
         ss << i->first << "=" << i->second << lineFeed;
      }
      ss << "}" << lineFeed;

      return ss.str();
   }

   CXplMessage CXplMessage::parse(const std::string& rawMessage)
   {
      CXplMessage msg;
      try
      {
         //we explode the string onto the { char
         std::vector<std::string> result;
         auto trimRawMessage = boost::trim_copy(rawMessage);
         if (trimRawMessage[trimRawMessage.size() - 1] == '\0')
            trimRawMessage.erase(trimRawMessage.end() - 1);

         boost::split(result, trimRawMessage, boost::is_any_of("{}"), boost::token_compress_on);


         //we trim each '\n' '\r' at the beggining or the end of each line
         //and we suppress empty lines due to previous trim
         int i = result.size() - 1;
         while (i >= 0)
         {
            std::string str = boost::trim_copy_if(result[i], boost::is_any_of("\n"));
            result[i] = str;
            if (result[i].empty())
               result.erase(result.begin() + i);
            i--;
         }

         //we must have 4 parts msg type id, header block, message shema and body
         if (result.size() != 4)
            throw CXplException("Malformed message");

         const auto messageTypeIdIndex = 0;
         const auto headerBlockIndex = 1;
         const auto messageSchemaIndex = 2;
         const auto bodyIndex = 3;

         //Message type identifier parsing
         if (result[messageTypeIdIndex] == XplCmdTypeIdentifier)
         {
            msg.setTypeIdentifier(kXplCommand);
         }
         else
         {
            if (result[messageTypeIdIndex] == XplStatTypeIdentifier)
            {
               msg.setTypeIdentifier(kXplStat);
            }
            else
            {
               if (result[messageTypeIdIndex] == XplTrigTypeIdentifier)
               {
                  msg.setTypeIdentifier(kXplTrigger);
               }
               else
               {
                  throw CXplException("Message type identifier is unknown");
               }
            }
         }

         //Header block parsing

         //we explode onto line feed char and also '\r' to be more peaceful
         std::vector<std::string> header;
         auto trimHeaderBlockIndex = boost::trim_copy(result[headerBlockIndex]);
         boost::split(header, trimHeaderBlockIndex, boost::is_any_of("\n\r"), boost::token_compress_on);

         //We must have 3 results : hop, source and dest
         if (header.size() != 3)
            throw CXplException("Header part must have 3 fields : hop, source and target");

         auto hopHasBeenFound = false;
         auto targetHasBeenFound = false;
         auto sourceHasBeenFound = false;

         for (auto headerLineIter = header.begin(); headerLineIter != header.end(); ++headerLineIter)
         {
            auto headerLine = *headerLineIter;
            std::vector<std::string> headerLineDecomposed;
            auto trimHeaderLine = boost::trim_copy(headerLine);
            boost::split(headerLineDecomposed, trimHeaderLine, boost::is_any_of("="), boost::token_compress_on);

            const auto nameIndex = 0;
            const auto valueIndex = 1;

            //we must have a couple of name=value -> 2 elements
            if (headerLineDecomposed.size() != 2)
               throw CXplException("Header block is incorrect");

            if (headerLineDecomposed[nameIndex] == XplHopHeader)
            {
               try
               {
                  const auto hop = std::stoi(headerLineDecomposed[valueIndex]);
                  msg.setHop(hop);
                  //we indicate that hop has been successfully parsed
                  hopHasBeenFound = true;
               }
               catch (const std::exception&)
               {
                  throw CXplException("Hop value must be an int");
               }
            }

            if (headerLineDecomposed[nameIndex] == XplSourceHeader)
            {
               msg.setSource(CXplActor::parse(headerLineDecomposed[valueIndex]));
               //we indicate that source has been successfully parsed
               sourceHasBeenFound = true;
            }

            if (headerLineDecomposed[nameIndex] == XplTargetHeader)
            {
               msg.setTarget(CXplActor::parse(headerLineDecomposed[valueIndex]));
               //we indicate that target has been successfully parsed
               targetHasBeenFound = true;
            }
         }

         //We must have 3 results : hop, source and dest
         if (!targetHasBeenFound || !sourceHasBeenFound || !hopHasBeenFound)
            throw CXplException("Header part must have 3 fields : hop, source and target");

         //Message Schema parsing
         msg.setMessageSchemaIdentifier(CXplMessageSchemaIdentifier::parse(result[messageSchemaIndex]));

         //Body parsing
         std::vector<std::string> body;
         auto trimBodyIndex = boost::trim_copy(result[bodyIndex]);
         boost::split(body, trimBodyIndex, boost::is_any_of("\n\r"), boost::token_compress_on);

         for (auto bodyIter = body.begin(); bodyIter != body.end(); ++bodyIter)
         {
            auto bodyLine = *bodyIter;
            std::vector<std::string> bodyLineDecomposed;
            auto trimBodyLine = boost::trim_copy(bodyLine);
            boost::split(bodyLineDecomposed, trimBodyLine, boost::is_any_of("="), boost::token_compress_on);

            //we must have a couple of name=value -> 2 elements
            if (bodyLineDecomposed.size() != 2)
               throw CXplException("Body block is incorrect");

            const auto nameIndex = 0;
            const auto valueIndex = 1;

            msg.addToBody(bodyLineDecomposed[nameIndex], bodyLineDecomposed[valueIndex]);
         }
         //SUCCESS
      }
      catch (std::exception& exc)
      {
         throw CXplException("Unable to parse message " + rawMessage + ". " + exc.what());
      }
      return msg;
   }


   CXplMessage CXplMessage::createHeartbeatRequest(const CXplActor& source)
   {
      CXplMessage msg(kXplCommand,
                      source,
                      CXplActor::createBroadcastActor(),
                      CXplMessageSchemaIdentifier::createHeartbeatRequestMessageSchemaIdentifer());
      msg.addToBody("command", "request");
      return msg;
   }
} // namespace xplcore


