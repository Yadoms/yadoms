#pragma once

namespace web
{
   namespace rest
   {
      class CPaging
      {
      public:
         CPaging(int page, int pagesCount, int pageSize);
         explicit CPaging(const CPaging& other) = default;

         CPaging& operator=(const CPaging& other);

         int page() const;
         int pagesCount() const;
         int pageSize() const;

      private:
         int m_page;
         int m_pagesCount;
         int m_pageSize;
      };
   } //namespace rest
} //namespace web 
