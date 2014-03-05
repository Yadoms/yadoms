#include "stdafx.h"
#include "XplHelper.h"
#include "XplMessageSchemaIdentifier.h"
#include "XplException.h"

namespace shared { namespace xpl
{

CXplMessageSchemaIdentifier::CXplMessageSchemaIdentifier()
{
}

CXplMessageSchemaIdentifier::CXplMessageSchemaIdentifier(const std::string & classId, const std::string & typeId)
{
   setClassId(classId);
   setTypeId(typeId);
}

CXplMessageSchemaIdentifier::CXplMessageSchemaIdentifier(const CXplMessageSchemaIdentifier & identifier)
{
   m_classId = identifier.getClassId();
   m_typeId = identifier.getTypeId();
}

CXplMessageSchemaIdentifier::~CXplMessageSchemaIdentifier()
{
}

void CXplMessageSchemaIdentifier::setClassId(const std::string & classId)
{
   CXplHelper::checkRules(CXplHelper::kClassId, classId);
   m_classId = classId;
}

std::string CXplMessageSchemaIdentifier::getClassId() const
{
   return m_classId;
}

void CXplMessageSchemaIdentifier::setTypeId(const std::string & typeId)
{
   CXplHelper::checkRules(CXplHelper::kTypeId, typeId);
   m_typeId = typeId;
}

std::string CXplMessageSchemaIdentifier::getTypeId() const
{
return m_typeId;
}

std::string CXplMessageSchemaIdentifier::toString() const
{
   return std::string(m_classId + "." + m_typeId);
}

CXplMessageSchemaIdentifier CXplMessageSchemaIdentifier::parse(const std::string & rawMessageSchemaIdentifier)
{
   std::string lineString = boost::trim_copy(rawMessageSchemaIdentifier);

   std::vector<std::string> line;
   boost::split(line, lineString, boost::is_any_of("."), boost::token_compress_on);

   //We must have 2 results : class Id, type Id
   if (line.size() != 2)
      throw CXplException("Message Shcema identifier must have 2 fields : class Id, type Id");

   const int classIdIndex = 0;
   const int typeIdIndex = 1;

   CXplMessageSchemaIdentifier msi;
   msi.setClassId(line[classIdIndex]);
   msi.setTypeId(line[typeIdIndex]);

   return msi;
}

CXplMessageSchemaIdentifier CXplMessageSchemaIdentifier::createHeartbeatMessageSchemaIdentifer()
{
   return CXplMessageSchemaIdentifier(CXplHelper::HeartbeatClassID, CXplHelper::HeartbeatTypeId);
}

bool CXplMessageSchemaIdentifier::isHeartbeatMessageSchemaIdentifier(const CXplMessageSchemaIdentifier & msi)
{
   return (msi.getClassId() == CXplHelper::HeartbeatClassID);
}

} } // namespace shared::xpl
