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
      initializeWith(rhs);
      return *this;
   }


   CDataContainer & CDataContainer::operator=(const std::string &rhs)
   {
      deserialize(rhs);
      return *this;
   }

   bool CDataContainer::hasValue(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      boost::optional<const boost::property_tree::ptree&> value = m_tree.get_child_optional(parameterName);
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

} // namespace shared
