#pragma once
#include <string>
#define MIN_F(a, b, c)  (fminf(a, fminf(b, c)))
#define MAX_F(a, b, c)  (fmaxf(a, fmaxf(b, c)))

class CColorConverter
{
public:
   struct rgb
   {
      unsigned long r;
      unsigned long g;
      unsigned long b;
   };

   struct hsv
   {
      unsigned long hue;
      unsigned long saturation;
      unsigned long value;
   };

   struct xy
   {
      float x;
      float y;
   };

   CColorConverter() = default;
   virtual ~CColorConverter() = default;

   static rgb hexToRgb(const std::string& hex);
   static hsv rgbToHsv(rgb& rgb);
   static xy rgbToXy(rgb& rgb);
};
