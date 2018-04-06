#include "stdafx.h"
#include "DataContainer.h"
#include <boost/property_tree/json_parser.hpp>
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
            assert(srcIt->value.GetType() == dstIt->value.GetType());
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
               dstIt->value = srcIt->value;
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


} // namespace shared