#include "stdafx.h"
#include "XplConstants.h"

namespace xplcore
{

static const std::string YadomsVendorId = "yadoms";

const std::string& CXplConstants::getYadomsVendorId()
{
   return YadomsVendorId;
}

} // namespace xplcore