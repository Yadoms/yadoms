#pragma once
#include <shared/ILocation.h>
#include <toPlugin.pb.h>

namespace plugin_cpp_api
{
   class CLocation :public shared::ILocation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    buffer : Protobuf buffer
      //--------------------------------------------------------------
      explicit CLocation(const plugin_IPC::toPlugin::Location& buffer);

      virtual ~CLocation();

      // shared::ILocation implementation
      double latitude() const override;
      double longitude() const override;
      double altitude() const override;
      // [END] shared::ILocation implementation

   private:
      const plugin_IPC::toPlugin::Location m_buffer;
   };
} // namespace plugin_cpp_api	


