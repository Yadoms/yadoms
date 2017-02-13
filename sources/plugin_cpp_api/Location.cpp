#include "stdafx.h"
#include "Location.h"


namespace plugin_cpp_api
{
   CLocation::CLocation(const plugin_IPC::toPlugin::Location& buffer)
      : m_buffer(buffer)
   {
   }

   CLocation::~CLocation()
   {
   }

   double CLocation::latitude() const
   {
      return m_buffer.latitude();
   }

   double CLocation::longitude() const
   {
      return m_buffer.longitude();
   }

   double CLocation::altitude() const
   {
      return m_buffer.longitude();
   }
} // namespace plugin_cpp_api	


