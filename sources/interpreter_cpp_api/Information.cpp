#include "stdafx.h"
#include "Information.h"


namespace interpreter_cpp_api
{
   CInformation::CInformation(boost::shared_ptr<const interpreter_IPC::toInterpreter::Information> buffer)
      : m_buffer(buffer),
        m_path(m_buffer->path())
   {
   }

   CInformation::~CInformation()
   {
   }

   const std::string& CInformation::getType() const
   {
      return m_buffer->type();
   }

   const std::string& CInformation::getName() const
   {
      return m_buffer->name();
   }

   const std::string& CInformation::getDescription() const
   {
      return m_buffer->description();
   }

   const shared::versioning::CVersion& CInformation::getVersion() const
   {
      static const shared::versioning::CVersion version(m_buffer->version());
      return version;
   }

   const std::string& CInformation::getAuthor() const
   {
      return m_buffer->author();
   }

   const std::string& CInformation::getUrl() const
   {
      return m_buffer->url();
   }

   const std::string& CInformation::getCredits() const
   {
      return m_buffer->credits();
   }

   bool CInformation::isSupportedOnThisPlatform() const
   {
      return m_buffer->supportedonthisplatform();
   }

   boost::shared_ptr<const shared::CDataContainer> CInformation::getPackage() const
   {
      return boost::make_shared<const shared::CDataContainer>(m_buffer->packagefilecontent());
   }

   const boost::filesystem::path& CInformation::getPath() const
   {
      return m_path;
   }
} // namespace interpreter_cpp_api	


