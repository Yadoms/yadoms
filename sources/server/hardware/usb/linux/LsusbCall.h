#pragma once
#include <string>
#include <vector>

namespace hardware
{
namespace usb
{
class CLsusbCall
{
public:
   CLsusbCall(const std::vector<std::string> &parameters = std::vector<std::string>());
   virtual ~CLsusbCall() = default;

   std::vector<std::string> execute(bool ignoreError = false);

private:
   const std::vector<std::string> m_parameters;
};
} // namespace usb
} // namespace hardware
