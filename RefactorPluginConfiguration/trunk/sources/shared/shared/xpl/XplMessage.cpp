#include "stdafx.h"
#include "XplMessage.h"
#include "XplActor.h"
#include "XplHelper.h"
#include "XplException.h"

namespace shared { namespace xpl
{

const std::string CXplMessage::XplCmdTypeIdentifier = "xpl-cmd";
const std::string CXplMessage::XplStatTypeIdentifier = "xpl-stat";
const std::string CXplMessage::XplTrigTypeIdentifier = "xpl-trig";

const std::string CXplMessage::XplHopHeader = "hop";
const std::string CXplMessage::XplSourceHeader = "source";
const std::string CXplMessage::XplTargetHeader = "target";

CXplMessage::CXplMessage()
{
   m_hop = 1;
   setTypeIdentifier(CXplMessage::kXplStat);
   setSource(CXplActor::createBroadcastActor());
   setTarget(CXplActor::createBroadcastActor());
   setMessageSchemaIdentifier(CXplMessageSchemaIdentifier("hbeat", "app"));
}

CXplMessage::CXplMessage(const CXplMessage::ETypeIdentifier & typeId, const CXplActor & source, const CXplActor & target, CXplMessageSchemaIdentifier  messageSchemaIdentifier)
{
   m_hop = 1;
   setTypeIdentifier(typeId);
   setSource(source);
   setTarget(target);
   setMessageSchemaIdentifier(messageSchemaIdentifier);
}

CXplMessage::CXplMessage(const CXplMessage & message) 
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

void CXplMessage::setTypeIdentifier(const CXplMessage::ETypeIdentifier & typeId)
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
      case CXplMessage::kXplCommand:
         return XplCmdTypeIdentifier;
         break;
      case CXplMessage::kXplStat:
         return XplStatTypeIdentifier;
         break;
      case CXplMessage::kXplTrigger:
         return XplTrigTypeIdentifier;
         break;
   }
   throw CXplException("Unknown Type Identifier");
}

void CXplMessage::setSource(const CXplActor & source)
{
   m_source = source;
}

CXplActor CXplMessage::getSource() const
{
   return m_source;
}

void CXplMessage::setTarget(const CXplActor & target)
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

void CXplMessage::setMessageSchemaIdentifier(const CXplMessageSchemaIdentifier & messageSchemaIdentifier)
{
   m_messageSchemaIdentifier = CXplMessageSchemaIdentifier(messageSchemaIdentifier);
}

const CXplMessageSchemaIdentifier & CXplMessage::getMessageSchemaIdentifier()
{
   return m_messageSchemaIdentifier;
}

void CXplMessage::addToBody(const std::string & name, const std::string & value)
{
   if (CXplHelper::isStructuralElementMatchRules(name) && (name.size() <= BodyNameMaxLength))
   {
      //the body element mustn't exist
      if (m_body.find(name) != m_body.end())
         throw CXplException("Element " + name + " can't be defined more than once");

      m_body[name] = value;
   }
   else
   {
      throw CXplException("name" + name + "doesn't match Xpl naming convention");
   }
}

const std::map<std::string, std::string> & CXplMessage::getBody()
{
   return m_body;
}

const std::string & CXplMessage::getBodyValue(const std::string & key)
{
   return m_body[key];
}

std::string CXplMessage::toString() const
{
   std::stringstream ss;

   char lineFeed = '\n';

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
   std::pair<std::string, std::string> bodyLine;
   BOOST_FOREACH(bodyLine, m_body)
   {
      ss << bodyLine.first << "=" << bodyLine.second << lineFeed;
   }
   ss << "}" << lineFeed;

   return ss.str();
}

CXplMessage CXplMessage::parse(const std::string & rawMessage)
{
   CXplMessage msg;
   try
   {
      //we explode the string onto the { char
      std::vector<std::string> result;
      std::string trimRawMessage = boost::trim_copy(rawMessage);
      boost::split(result, trimRawMessage, boost::is_any_of("{}"), boost::token_compress_on);

      //we trim each '\n' '\r' at the beggining or the end of each line
      //and we suppress empty lines due to previous trim
      int i = result.size() - 1;
      while (i >= 0)
      {
         std::string str = boost::trim_copy_if(result[i], boost::is_any_of("\n"));
         result[i] = str;
         if (result[i] == "")
            result.erase(result.begin()+i);
         i--;
      }

      //we must have 4 parts msg type id, header block, message shema and body
      if (result.size() != 4)
         throw CXplException("Malformed message");

      const int messageTypeIdIndex = 0;
      const int headerBlockIndex = 1;
      const int messageSchemaIndex = 2;
      const int bodyIndex = 3;

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
      std::string trimHeaderBlockIndex = boost::trim_copy(result[headerBlockIndex]);
      boost::split(header, trimHeaderBlockIndex, boost::is_any_of("\n\r"), boost::token_compress_on);

      //We must have 3 results : hop, source and dest
      if (header.size() != 3)
         throw CXplException("Header part must have 3 fields : hop, source and target");

      bool hopHasBeenFound = false;
      bool targetHasBeenFound = false;
      bool sourceHasBeenFound = false;

      BOOST_FOREACH(std::string headerLine, header)
      {
         std::vector<std::string> headerLineDecomposed;
         std::string trimHeaderLine = boost::trim_copy(headerLine);
         boost::split(headerLineDecomposed, trimHeaderLine, boost::is_any_of("="), boost::token_compress_on);
      
         const int nameIndex = 0;
         const int valueIndex = 1;

         //we must have a couple of name=value -> 2 elements
         if (headerLineDecomposed.size() != 2)
            throw CXplException("Header block is incorrect");
         
         if (headerLineDecomposed[nameIndex] == XplHopHeader)
         {
            int hop;
            if (!tryParse<int>(headerLineDecomposed[valueIndex], hop))
               throw CXplException("Hop value must be an int");
         
            msg.setHop(hop);
            //we indicate that hop has been successfully parsed
            hopHasBeenFound = true;
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
      std::string trimBodyIndex = boost::trim_copy(result[bodyIndex]);
      boost::split(body, trimBodyIndex, boost::is_any_of("\n\r"), boost::token_compress_on);
      BOOST_FOREACH(std::string bodyLine, body)
      {
         std::vector<std::string> bodyLineDecomposed;
	 std::string trimBodyLine = boost::trim_copy(bodyLine);
         boost::split(bodyLineDecomposed, trimBodyLine, boost::is_any_of("="), boost::token_compress_on);

         //we must have a couple of name=value -> 2 elements
         if (bodyLineDecomposed.size() != 2)
            throw CXplException("Body block is incorrect");
         
         const int nameIndex = 0;
         const int valueIndex = 1;

         msg.addToBody(bodyLineDecomposed[nameIndex], bodyLineDecomposed[valueIndex]);
      }
      //SUCCESS
   }
   catch (std::exception & exc)
   {
      throw CXplException("Unable to parse message " + rawMessage + ". " + exc.what());
   }
   return msg;
}

} } // namespace shared::xpl
