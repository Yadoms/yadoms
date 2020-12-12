#pragma once
#include <string>
#include "Rgb.h"
#include "Hsv.h"
#include "Xy.h"
#define MIN_F(a, b, c)  (fminf(a, fminf(b, c)))
#define MAX_F(a, b, c)  (fmaxf(a, fmaxf(b, c)))

class CColorConverter
{
public:

   CColorConverter() = default;
   virtual ~CColorConverter() = default;

   static CRgb hexToRgb(const std::string& hex);
   static CHsv rgbToHsv(CRgb& rgb);
   static CXy rgbToXy(CRgb& rgb);
};
