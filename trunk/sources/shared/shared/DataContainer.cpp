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

   CDataContainer & CDataContainer::operator =(const CDataContainer & rhs)
   {
      m_tree = rhs.m_tree;
      return *this;
   }

   bool CDataContainer::hasValue(const std::string& parameterName) const
   {
      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(parameterName);
      return !!value;
   }


   inline CDataContainer CDataContainer::getChild(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree child = m_tree.get_child(parameterName);

		 CDataContainer t;
		 t.m_tree = child;
         return t;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   inline void CDataContainer::setChild(const std::string& parameterName, const CDataContainer & value)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.add_child(parameterName, value.m_tree);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   std::string CDataContainer::serialize() const
   {
      std::ostringstream buf;
      boost::property_tree::json_parser::write_json(buf, m_tree, false);
      return buf.str();
   }

   void CDataContainer::deserialize(const std::string & data)
   {
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
      boost::property_tree::json_parser::write_json(filename, m_tree);
   }

   void CDataContainer::deserializeFromFile(const std::string & file)
   {
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

   bool CDataContainer::operator ==(const CDataContainer &rhs) const
   {
      return serialize() == rhs.serialize();
   }

   bool CDataContainer::operator !=(const CDataContainer &rhs) const
   {
      return serialize() != rhs.serialize();
   }

   void CDataContainer::set(const CDataContainer &rhs)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);
      m_tree = rhs.m_tree;
   }


   bool CDataContainer::empty() const
   {
      return m_tree.empty();
   }

} // namespace shared
