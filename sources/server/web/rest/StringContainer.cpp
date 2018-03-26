#include "stdafx.h"
#include "StringContainer.h"

namespace web { namespace rest { 

   CStringContainer::CStringContainer(const std::string & content)
      :m_content(content)
   {
   }

   CStringContainer::~CStringContainer()
   {}

   std::string CStringContainer::serialize() const
   {
      return m_content;
   }

   void CStringContainer::deserialize(const std::string & data)
   {
      m_content = data;
   }

} //namespace rest
} //namespace web 
