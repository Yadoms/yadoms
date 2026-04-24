#include "stdafx.h"
#include "Paging.h"

using namespace web::rest;

CPaging::CPaging(int page, int pagesCount, int pageSize)
   : m_page(page), m_pagesCount(pagesCount), m_pageSize(pageSize)
{
}

CPaging& CPaging::operator=(const CPaging& other)
{
   if (this == &other)
      return *this;

   m_page = other.m_page;
   m_pagesCount = other.m_pagesCount;
   m_pageSize = other.m_pageSize;
   return *this;
}

int CPaging::page() const
{
   return m_page;
}

int CPaging::pagesCount() const
{
   return m_pagesCount;
}

int CPaging::pageSize() const
{
   return m_pageSize;
}
