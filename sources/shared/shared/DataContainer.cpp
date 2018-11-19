#include "stdafx.h"
#include "DataContainer.h"
#include <shared/exception/JSONParse.hpp>
#include "exception/EmptyResult.hpp"
#include "rapidJson/writer.h"
#include "rapidJson/istreamwrapper.h"
#include "rapidJson/ostreamwrapper.h"
#include "rapidJson/prettywriter.h"
#include "rapidJson/pointer.h"

namespace shared
{
   const CDataContainer CDataContainer::EmptyContainer;

   CDataContainer::CDataContainer()
   {
      m_tree.SetObject();

   }

   CDataContainer::CDataContainer(const std::string & initialData)
   {
      m_tree.SetObject();
      CDataContainer::deserialize(initialData);
   }

   CDataContainer::CDataContainer(rapidjson::Value & d)
   {
      m_tree.SetObject();
      rapidjson::Document::AllocatorType& a = m_tree.GetAllocator();
      m_tree.CopyFrom(d, a);
   }

   CDataContainer::CDataContainer(rapidjson::Value * d)
   {
      m_tree.SetObject();
      m_tree.CopyFrom(*d, m_tree.GetAllocator());
   }

   CDataContainer::CDataContainer(rapidjson::Document & d)
   {
      m_tree.SetObject();
      m_tree.CopyFrom(d, m_tree.GetAllocator());
   }


   CDataContainer::CDataContainer(const std::map<std::string, std::string> & initialData)
   {
      m_tree.SetObject();
      std::map<std::string, std::string>::const_iterator i;
      for (i = initialData.begin(); i != initialData.end(); ++i)
         set(i->first, i->second);
   }


   CDataContainer::~CDataContainer()
   {

   }


   CDataContainer::CDataContainer(const CDataContainer & initialData)
   {
      m_tree.SetObject();
      m_tree.CopyFrom(initialData.m_tree, m_tree.GetAllocator());
   }


   CDataContainer::CDataContainer(const rapidjson::Document & initialTree)
   {
      m_tree.SetObject();
      m_tree.CopyFrom(initialTree, m_tree.GetAllocator());
   }

   CDataContainer & CDataContainer::operator =(const CDataContainer & rhs)
   {
      initializeWith(rhs);
      return *this;
   }


   CDataContainer & CDataContainer::operator=(const std::string &rhs)
   {
      deserialize(rhs);
      return *this;
   }



   std::ostream& operator << (std::ostream& os, const CDataContainer & dc)
   {
      os << dc.serialize();
      return os;
   }

   std::istream& operator >> (std::istream& is, CDataContainer & dc)
   {
      boost::lock_guard<boost::mutex> lock(dc.m_treeMutex);

      dc.m_tree.RemoveAllMembers();

      rapidjson::IStreamWrapper isw(is);

      dc.m_tree.ParseStream(isw);
      return is;
   }


   bool CDataContainer::exists(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value* found = findValue(parameterName, pathChar);
      return (found != NULL);
   }


   bool CDataContainer::containsChild(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
         return !found->IsNull() && found->IsObject();
      return false;
   }

   bool CDataContainer::containsValue(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::Value* found = findValue(parameterName, pathChar);

      if (found != NULL && !found->IsNull())
      {
         return found->IsBool() || found->IsDouble() || found->IsFloat() || found->IsInt() || found->IsInt64() ||
            found->IsNumber() || found->IsString() || found->IsUint() || found->IsUint64();
      }
      return false;
   }


   std::string CDataContainer::serialize() const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      rapidjson::StringBuffer sb;
      rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
      m_tree.Accept(writer);

      return sb.GetString();
   }

   void CDataContainer::deserialize(const std::string & data)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      m_tree.RemoveAllMembers();

      if (m_tree.Parse(data.c_str()).HasParseError())
         throw exception::CException("Fail to parse Json");
   }

   void CDataContainer::serializeToFile(const std::string & filename) const
   {
      std::ofstream ofs(filename);
      rapidjson::OStreamWrapper osw(ofs);
      rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
      m_tree.Accept(writer);
   }

   void CDataContainer::deserializeFromFile(const std::string & file)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      m_tree.RemoveAllMembers();

      std::ifstream ifs(file);
      rapidjson::IStreamWrapper isw(ifs);

      m_tree.ParseStream(isw);
   }

   void CDataContainer::extractContent(CDataContainer & container) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      container.m_tree.CopyFrom(m_tree, container.m_tree.GetAllocator());
   }

   void CDataContainer::fillFromContent(const CDataContainer & initialData)
   {
      initializeWith(initialData);
   }




   bool CDataContainer::operator ==(const CDataContainer &rhs) const
   {
      return serialize() == rhs.serialize();
   }

   bool CDataContainer::operator !=(const CDataContainer &rhs) const
   {
      return serialize() != rhs.serialize();
   }

   void CDataContainer::initializeWith(const CDataContainer &rhs)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      m_tree.CopyFrom(rhs.m_tree, m_tree.GetAllocator());
   }


   bool CDataContainer::empty() const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      return m_tree.MemberCount() == 0;
   }

   void CDataContainer::printToLog(std::ostream& os) const
   {
      os << std::endl;

      rapidjson::OStreamWrapper osw(os);
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      m_tree.Accept(writer);
   }

   std::string CDataContainer::generatePath(const std::string & parameterName, const char pathChar) const
   {
      std::string res = "/";
      int c = parameterName.size();
      const char * s = parameterName.c_str();
      for (int i = 0; i < c; ++i)
         res += (s[i] == pathChar) ? '/' : s[i];
      return res;
   }

   void CDataContainer::set(const char* parameterName, const char* value, const char pathChar)
   {
      std::string strParamName(parameterName);
      std::string strValue(value);
      set<std::string>(strParamName, strValue, pathChar);
   }


   void CDataContainer::set(const std::string & parameterName, const char* value, const char pathChar)
   {
      std::string strValue(value);
      set<std::string>(parameterName, strValue, pathChar);
   }

   const char* CDataContainer::get(const std::string & parameterName, const char pathChar) const
   {
      return get<std::string>(parameterName, pathChar).c_str();
   }

   std::map<std::string, std::string> CDataContainer::getAsMap(const std::string& parameterName, const char pathChar) const
   {
      std::map<std::string, std::string> result;

      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         for (rapidjson::Value::ConstMemberIterator i = found->MemberBegin(); i != found->MemberEnd(); ++i)
         {
            result[i->name.GetString()] = i->value.GetString();
         }
      }

      return result;
   }

   std::vector<std::string> CDataContainer::getKeys(const std::string& parameterName, const char pathChar) const
   {
      std::vector<std::string> result;
      rapidjson::Value* found = findValue(parameterName, pathChar);
      if (found)
      {
         for (rapidjson::Value::ConstMemberIterator i = found->MemberBegin(); i != found->MemberEnd(); ++i)
         {
            result.push_back(i->name.GetString());
         }
      }
      return result;
   }

   rapidjson::Value* CDataContainer::findValue(const std::string& parameterName, const char pathChar) const
   {
      if (parameterName.empty())
      {
         return (rapidjson::Document *)&m_tree;
      }


      std::string path = generatePath(parameterName, pathChar);
      return (rapidjson::Value*)rapidjson::Pointer(path.c_str()).Get(m_tree);
   }

   CDataContainer CDataContainer::find(const std::string& parameterName, boost::function<bool(const CDataContainer&)> whereFct, const char pathChar) const
   {
      for (const auto& key : getKeys(parameterName, pathChar))
      {
         const auto path = parameterName + pathChar + key;
         const auto container = get<CDataContainer>(path);
         if (whereFct(container))
         {
            CDataContainer result;
            result.set(key, container);
            return result;
         }
      }
      throw exception::CEmptyResult("No parameter matches criteria");
   }
   
   void CDataContainer::mergeObjects(rapidjson::Value &dstObject, rapidjson::Value &srcObject, rapidjson::Document::AllocatorType &allocator)
   {
      for (auto srcIt = srcObject.MemberBegin(); srcIt != srcObject.MemberEnd(); ++srcIt)
      {
         auto dstIt = dstObject.FindMember(srcIt->name);
         if (dstIt != dstObject.MemberEnd())
         {
            if (srcIt->value.IsArray())
            {
               for (auto arrayIt = srcIt->value.Begin(); arrayIt != srcIt->value.End(); ++arrayIt)
               {
                  dstIt->value.PushBack(*arrayIt, allocator);
               }
            }
            else if (srcIt->value.IsObject())
            {
               mergeObjects(dstIt->value, srcIt->value, allocator);
            }
            else
            {
				//check type match, but also check some specific cases (kTrueType and kFalseType are booleans; and many number combination can alsom match)
				if (srcIt->value.GetType() == dstIt->value.GetType() ||
					(srcIt->value.IsBool() && dstIt->value.IsBool()) ||
					(srcIt->value.IsNumber() && dstIt->value.IsNumber()))
				{
					dstIt->value = srcIt->value;
				}
				else
				{
					//types don't match, try to make concordance
					switch (dstIt->value.GetType())
					{
					case rapidjson::kFalseType:
					case rapidjson::kTrueType:
						dstIt->value.SetBool(ConvertToBool(&srcIt->value));
						break;
					case rapidjson::kStringType:
						dstIt->value.SetString(ConvertToString(&srcIt->value), allocator);
						break;
					case rapidjson::kNumberType:
						if(dstIt->value.IsInt())
							dstIt->value.SetInt(ConvertToInt(&srcIt->value));
						else if (dstIt->value.IsInt64())
							dstIt->value.SetInt64(ConvertToInt64(&srcIt->value));
						else if (dstIt->value.IsUint())
							dstIt->value.SetUint(ConvertToUInt(&srcIt->value));
						else if (dstIt->value.IsUint64())
							dstIt->value.SetUint64(ConvertToUInt64(&srcIt->value));
						else if (dstIt->value.IsDouble())
							dstIt->value.SetDouble(ConvertToDouble(&srcIt->value));
						else if (dstIt->value.IsFloat())
							dstIt->value.SetFloat(ConvertToFloat(&srcIt->value));
						else
							throw exception::CInvalidParameter("Value is not a valid type");
						break;
					default:
						throw exception::CInvalidParameter("Value is not a valid type");
						break;
					}
				}
            }
         }
         else
         {
            dstObject.AddMember(srcIt->name, srcIt->value, allocator);
         }
      }
   }

   void CDataContainer::mergeFrom(const CDataContainer& source)
   {
      auto& allocator = m_tree.GetAllocator();
      mergeObjects(m_tree, *source.getPointer(), allocator);
   }

   void CDataContainer::setNull(const std::string& parameterName, const char pathChar)
   {
      auto v = this->findValue(parameterName, pathChar);
      if(v)
         v->SetNull();
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }

   bool CDataContainer::isNull(const std::string& parameterName, const char pathChar)
   {
      auto v = this->findValue(parameterName, pathChar);
      if (v)
         return v->IsNull();
      throw exception::CInvalidParameter(parameterName + " : is not found");
   }

   std::string CDataContainer::ConvertToString(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString())
            return std::string(v->GetString());
     
         if (v->IsBool())
            return v->GetBool() ? "true" : "false";

         if (v->IsDouble())
            return boost::lexical_cast<std::string>(v->GetDouble());
         if (v->IsFalse())
            return "false";
         if (v->IsFloat())
            return boost::lexical_cast<std::string>(v->GetFloat());
         if (v->IsInt())
            return boost::lexical_cast<std::string>(v->GetInt());
         if (v->IsInt64())
            return boost::lexical_cast<std::string>(v->GetInt64());
         if (v->IsTrue())
            return "true";
         if (v->IsUint())
            return boost::lexical_cast<std::string>(v->GetUint());
         if (v->IsUint64())
            return boost::lexical_cast<std::string>(v->GetUint64());

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }

   bool CDataContainer::ConvertToBool(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::iequals(s, "true") || boost::iequals(s, "1");
         }
         if (v->IsBool())
            return v->GetBool();
         if (v->IsDouble())
            return v->GetDouble() != 0.0f;
         if (v->IsFalse())
            return false;
         if (v->IsFloat())
            return v->GetFloat() != 0.0f;
         if (v->IsInt())
            return v->GetInt() != 0;
         if (v->IsInt64())
            return v->GetInt64() != 0;
         if (v->IsTrue())
            return true;
         if (v->IsUint())
            return v->GetUint() != 0;
         if (v->IsUint64())
            return v->GetUint64() != 0;

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }


   int CDataContainer::ConvertToInt(rapidjson::Value* v) 
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<int>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (int)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (int)v->GetFloat();
         if (v->IsInt())
            return v->GetInt();
         if (v->IsInt64())
            return (int)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return v->GetUint();
         if (v->IsUint64())
            return (int)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }

   int64_t CDataContainer::ConvertToInt64(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<int64_t>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (int64_t)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (int64_t)v->GetFloat();
         if (v->IsInt())
            return v->GetInt();
         if (v->IsInt64())
            return v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return v->GetUint();
         if (v->IsUint64())
            return v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }

   char CDataContainer::ConvertToByte(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return s[0];
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (char)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (char)v->GetFloat();
         if (v->IsInt())
            return (char)v->GetInt();
         if (v->IsInt64())
            return (char)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return (char)v->GetUint();
         if (v->IsUint64())
            return (char)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }

   short CDataContainer::ConvertToShort(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<short>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (short)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (short)v->GetFloat();
         if (v->IsInt())
            return (short)v->GetInt();
         if (v->IsInt64())
            return (short)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return (short)v->GetUint();
         if (v->IsUint64())
            return (short)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
   unsigned int CDataContainer::ConvertToUInt(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<unsigned int>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (unsigned int)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (unsigned int)v->GetFloat();
         if (v->IsInt())
            return v->GetInt();
         if (v->IsInt64())
            return (unsigned int)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return (unsigned int)v->GetUint();
         if (v->IsUint64())
            return (unsigned int)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
   uint64_t CDataContainer::ConvertToUInt64(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<uint64_t>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (uint64_t)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (uint64_t)v->GetFloat();
         if (v->IsInt())
            return v->GetInt();
         if (v->IsInt64())
            return v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return v->GetUint();
         if (v->IsUint64())
            return v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
   unsigned char CDataContainer::ConvertToUByte(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<unsigned char>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (unsigned char)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (unsigned char)v->GetFloat();
         if (v->IsInt())
            return (unsigned char)v->GetInt();
         if (v->IsInt64())
            return (unsigned char)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return (unsigned char)v->GetUint();
         if (v->IsUint64())
            return (unsigned char)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
   unsigned short CDataContainer::ConvertToUShort(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<unsigned short>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1 : 0;
         if (v->IsDouble())
            return (unsigned short)v->GetDouble();
         if (v->IsFalse())
            return 0;
         if (v->IsFloat())
            return (unsigned short)v->GetFloat();
         if (v->IsInt())
            return (unsigned short)v->GetInt();
         if (v->IsInt64())
            return (unsigned short)v->GetInt64();
         if (v->IsTrue())
            return 1;
         if (v->IsUint())
            return (unsigned short)v->GetUint();
         if (v->IsUint64())
            return (unsigned short)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }

   float CDataContainer::ConvertToFloat(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<float>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1.0f : 0.0f;
         if (v->IsDouble())
            return (float)v->GetDouble();
         if (v->IsFalse())
            return 0.0f;
         if (v->IsFloat())
            return v->GetFloat();
         if (v->IsInt())
            return (float)v->GetInt();
         if (v->IsInt64())
            return (float)v->GetInt64();
         if (v->IsTrue())
            return 1.0f;
         if (v->IsUint())
            return (float)v->GetUint();
         if (v->IsUint64())
            return (float)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
   double CDataContainer::ConvertToDouble(rapidjson::Value* v)
   {
      if (v)
      {
         if (v->IsString()) {
            std::string s = v->GetString();
            return boost::lexical_cast<double>(s);
         }
         if (v->IsBool())
            return v->GetBool() ? 1.0f : 0.0f;
         if (v->IsDouble())
            return v->GetDouble();
         if (v->IsFalse())
            return 0.0f;
         if (v->IsFloat())
            return v->GetFloat();
         if (v->IsInt())
            return v->GetInt();
         if (v->IsInt64())
            return (double)v->GetInt64();
         if (v->IsTrue())
            return 1.0f;
         if (v->IsUint())
            return v->GetUint();
         if (v->IsUint64())
            return (double)v->GetUint64();

         throw exception::CInvalidParameter("Value is not a valid type");
      }
      throw exception::CInvalidParameter("Parameter is null");
   }
} // namespace shared