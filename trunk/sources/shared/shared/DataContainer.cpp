#include "stdafx.h"
#include "DataContainer.h"
#include "Log.h"

namespace shared
{
   const CDataContainer CDataContainer::EmptyContainer;

   CDataContainer::CDataContainer()
   {

   }

   CDataContainer::CDataContainer(const std::string & initialData)
   {
      deserialize(initialData);
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


   bool CDataContainer::hasValue(const std::string& parameterName, const char pathChar) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(generatePath(parameterName, pathChar));
      return !!value;
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
         throw exception::CInvalidParameter(e.what());
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
         throw exception::CException(e.what());
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

   void CDataContainer::printToLog() const
   {
      YADOMS_LOG(info) << "| TREE START";
      printToLog(m_tree, 0);
      YADOMS_LOG(info) << "| TREE END";
   }

   void CDataContainer::printToLog(const boost::property_tree::ptree & pt, const int deep) const
   {
      boost::property_tree::ptree::const_iterator end = pt.end();

      std::string prefix = "|-";
      for (int i = 0; i < deep; ++i)
         prefix += "---";
      for (boost::property_tree::ptree::const_iterator it = pt.begin(); it != end; ++it) 
      {
         YADOMS_LOG(info) << prefix << it->first << ": " << it->second.get_value<std::string>() << std::endl;
         printToLog(it->second, deep+1);
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

   void CDataContainer::set(const std::string & parameterName, const char* value, const char pathChar)
   {
      std::string strValue(value);
      set<std::string>(parameterName, strValue, pathChar);
   }

   const char* CDataContainer::get(const char* parameterName, const char pathChar)
   {
      std::string strParamName(parameterName);
      return get<std::string>(strParamName, pathChar).c_str();
   }

   const char* CDataContainer::get(const std::string & parameterName, const char pathChar)
   {
      return get<std::string>(parameterName, pathChar).c_str();
   }


} // namespace shared
