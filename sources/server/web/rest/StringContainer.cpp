﻿#include "stdafx.h"
#include "StringContainer.h"

#include <utility>

namespace web
{
   namespace rest
   {
      CStringContainer::CStringContainer(std::string content)
         : m_content(std::move(content))
      {
      }

      std::string CStringContainer::serialize() const
      {
         return m_content;
      }

      void CStringContainer::deserialize(const std::string& data)
      {
         m_content = data;
      }
   } //namespace rest
} //namespace web 
