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
   rgb rgbColor{0,0,0};
   rgbColor.r = std::stoul(match[1].str(), nullptr, 16);
   rgbColor.g = std::stoul(match[2].str(), nullptr, 16);
   rgbColor.b = std::stoul(match[3].str(), nullptr, 16);
   return rgbColor;
}
