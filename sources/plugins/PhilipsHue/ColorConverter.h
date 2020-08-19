#pragma once
#include <string>

class CColorConverter
{
public:
   struct rgb
   {
      unsigned long r;
      unsigned long g;
      unsigned long b;
   };

   CColorConverter() = default;
   virtual ~CColorConverter() = default;

   static rgb hexToRgb(const std::string& hex);
};
