#include "stdafx.h"
#include "DataContainer.h"
#include <boost/property_tree/json_parser.hpp>
#include <shared/exception/JSONParse.hpp>
#include "exception/EmptyResult.hpp"

namespace shared
{
   const CDataContainer CDataContainer::EmptyContainer;

   CDataContainer::CDataContainer()
   {

   }

   CDataContainer::CDataContainer(const std::string & initialData)
   {
      CDataContainer::deserialize(initialData);
   }

   CDataContainer::CDataContainer(const std::map<std::string, std::string> & initialData)
   {
      std::map<std::string, std::string>::const_iterator i;
      for (i = initialData.begin(); i != initialData.end(); ++i)
         set(i->first, i->second);
   }


   CDataContainer::~CDataContainer()
   {

   }


   CDataContainer::CDataContainer(const CDataContainer & initialData)
   {
      m_tree = initialData.m_tree;
   }

   
   CDataContainer::CDataContainer(const boost::property_tree::ptree & initialTree)
   {
      m_tree = initialTree;
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

      dc.m_tree.clear();

      try
      {
         boost::property_tree::json_parser::read_json(is, dc.m_tree);
      }
      catch (boost::property_tree::json_parser::json_parser_error& e)
      {
         throw exception::CInvalidParameter(e.what());
      }

      return is;
   }


   bool CDataContainer::exists(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(generatePath(parameterName, pathChar));
      return !!value;
   }


   bool CDataContainer::containsChild(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(generatePath(parameterName, pathChar));
      if (!!value)
      {
         return !value->empty() && value->data().empty();
      }
      else
      {
         return false;
      }
   }

   bool CDataContainer::containsValue(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(generatePath(parameterName, pathChar));
      if (!!value)
      {
         return value->empty() && !value->data().empty();
      }
      else
      {
         return false;
      }
   }


   std::string CDataContainer::serialize() const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::ostringstream buf;
      boost::property_tree::json_parser::write_json(buf, m_tree, false);
      return buf.str();
   }

   void CDataContainer::deserialize(const std::string & data)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      m_tree.clear();

      if (data.empty())
         return;

      std::istringstream is(data);
      try
      {
         boost::property_tree::json_parser::read_json(is, m_tree);
      }
      catch (boost::property_tree::json_parser::json_parser_error& e)
      {
         throw exception::CJSONParse(e.what(), e.line());
      }
   }

   void CDataContainer::serializeToFile(const std::string & filename) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::property_tree::json_parser::write_json(filename, m_tree);
   }

   void CDataContainer::deserializeFromFile(const std::string & file)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      m_tree.clear();

      try
      {
         boost::property_tree::json_parser::read_json(file, m_tree);
      }
      catch (boost::property_tree::json_parser::json_parser_error& e)
      {
         throw exception::CJSONParse(e.what(), e.line());
      }
   }

   void CDataContainer::extractContent(CDataContainer & container) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      container.m_tree = m_tree;
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
      m_tree = rhs.m_tree;
   }


   bool CDataContainer::empty() const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      return m_tree.empty();
   }

   void CDataContainer::printToLog(std::ostream& os) const
   {
      os << std::endl;
      os << "| TREE START" << std::endl;
      printToLog(m_tree, 0, os);
      os << "| TREE END" << std::endl;
   }

   void CDataContainer::printToLog(const boost::property_tree::ptree & pt, const int deep, std::ostream& os) const
   {
      std::string prefix;
      for (auto i = 0; i < deep; ++i)
         prefix += "   ";
		prefix += "|-";
      for (const auto it : pt)
      {
         os << prefix << it.first << " : " << it.second.get_value<std::string>() << std::endl;
         printToLog(it.second, deep+1, os);
      }
   }

   boost::property_tree::ptree::path_type CDataContainer::generatePath(const std::string & parameterName, const char pathChar) const
   {
      return boost::property_tree::ptree::path_type(parameterName, pathChar);
   }

   void CDataContainer::set(const char* parameterName, const char* value, const char pathChar)
   {
      std::string strParamName(parameterName);
      std::string strValue(value);
      set<std::string>(strParamName, strValue, pathChar);
   }

   std::string CDataContainer::getKey() const
   {
      return m_tree.front().first;
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
      for (const auto i : m_tree.get_child(generatePath(parameterName, pathChar)))
         result[i.first] = i.second.data();
      return result;
   }

   std::vector<std::string> CDataContainer::getKeys(const std::string& parameterName, const char pathChar) const
   {
      std::vector<std::string> result;
      for (const auto& i : m_tree.get_child(generatePath(parameterName, pathChar)))
         result.push_back(i.first);
      return result;
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


} // namespace shared
