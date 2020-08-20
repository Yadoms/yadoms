#include "ColorConverter.h"
#include <regex>

CColorConverter::rgb CColorConverter::hexToRgb(const std::string& hex)
{
   const std::regex pattern("([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
   std::smatch match;
   if (!std::regex_match(hex, match, pattern))
   {
      throw std::runtime_error("hexToRgb : Hex value is not valid");
   }
   rgb rgbColor{0, 0, 0};
   rgbColor.r = std::stoul(match[1].str(), nullptr, 16);
   rgbColor.g = std::stoul(match[2].str(), nullptr, 16);
   rgbColor.b = std::stoul(match[3].str(), nullptr, 16);
   return rgbColor;
}


CColorConverter::hsv CColorConverter::rgbToHsv(rgb& rgb)
{
   const auto r = static_cast<float>(rgb.r) / 255.0f;
   const auto g = static_cast<float>(rgb.g) / 255.0f;
   const auto b = static_cast<float>(rgb.b) / 255.0f;

   float h, s; // h:0-360.0, s:0.0-1.0, 

   const auto max = MAX_F(r, g, b);
   const auto min = MIN_F(r, g, b);

   const auto v = max; //v:0.0-1.0

   if (max == 0.0f)
   {
      s = 0;
      h = 0;
   }
   else if (max - min == 0.0f)
   {
      s = 0;
      h = 0;
   }
   else
   {
      s = (max - min) / max;

      if (max == r)
      {
         h = 60 * ((g - b) / (max - min)) + 0;
      }
      else if (max == g)
      {
         h = 60 * ((b - r) / (max - min)) + 120;
      }
      else
      {
         h = 60 * ((r - g) / (max - min)) + 240;
      }
   }

   if (h < 0) h += 360.0f;
   hsv hsv{0, 0, 0};
   hsv.hue = static_cast<unsigned long>(h / 2); //  0-180
   hsv.saturation = static_cast<unsigned long>(s * 255); // 0-255
   hsv.value = static_cast<unsigned long>(v * 255); // 0-255
   return hsv;
}

CColorConverter::xy CColorConverter::rgbToXy(rgb& rgb)
{
   auto red = static_cast<float>(rgb.r) / 255;
   auto green = static_cast<float>(rgb.g) / 255;
   auto blue = static_cast<float>(rgb.b) / 255;

   if (red > 0.04045)
   {
      red = pow((red + 0.055) / (1.0 + 0.055), 2.4);
   }
   else red = (red / 12.92);

   if (green > 0.04045)
   {
      green = pow((green + 0.055) / (1.0 + 0.055), 2.4);
   }
   else green = (green / 12.92);

   if (blue > 0.04045)
   {
      blue = pow((blue + 0.055) / (1.0 + 0.055), 2.4);
   }
   else blue = blue / 12.92;

   const auto x = red * 0.664511 + green * 0.154324 + blue * 0.162028;
   const auto y = red * 0.283881 + green * 0.668433 + blue * 0.047685;
   const auto z = red * 0.000088 + green * 0.072310 + blue * 0.986039;

   xy xy{0.0, 0.0};
   xy.x = x / (x + y + z);
   xy.y = y / (x + y + z);

   return xy;
}
