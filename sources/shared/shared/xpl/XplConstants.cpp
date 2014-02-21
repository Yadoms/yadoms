#include "stdafx.h"
#include "XplConstants.h"

namespace shared { namespace xpl
{

static const std::string YadomsVendorId = "yadoms";

const std::string& CXplConstants::getYadomsVendorId()
{
   return YadomsVendorId;
}

} } // namespace shared::xpl