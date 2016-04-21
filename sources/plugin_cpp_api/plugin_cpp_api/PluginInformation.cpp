#include "stdafx.h"
#include "PluginInformation.h"


CPluginInformation::CPluginInformation(boost::shared_ptr<const pbPluginInformation::Information> buffer)
   : m_buffer(buffer)
{
}

CPluginInformation::~CPluginInformation()
{
}

const std::string& CPluginInformation::getType() const
{
   return m_buffer->type();
}

const std::string& CPluginInformation::getVersion() const
{
   return m_buffer->version();
}

shared::versioning::EReleaseType CPluginInformation::getReleaseType() const
{
   switch (m_buffer->releasetype())
   {
   case pbPluginInformation::Information_EReleaseType_kStable: return shared::versioning::EReleaseType::kStable;
   case pbPluginInformation::Information_EReleaseType_kTesting: return shared::versioning::EReleaseType::kTesting;
   default: return shared::versioning::EReleaseType::kBeta;
   }
}

const std::string& CPluginInformation::getAuthor() const
{
   return m_buffer->author();
}

const std::string& CPluginInformation::getUrl() const
{
   return m_buffer->url();
}

std::string CPluginInformation::getIdentity() const
{
   return m_buffer->identity();
}

std::string CPluginInformation::toString() const
{
   return m_buffer->tostring();
}

bool CPluginInformation::isSupportedOnThisPlatform() const
{
   return m_buffer->supportedonthisplatform();
}

bool CPluginInformation::getSupportManuallyCreatedDevice() const
{
   return m_buffer->supportmanuallycreateddevice();
}

shared::CDataContainer CPluginInformation::getPackage() const
{
   return shared::CDataContainer(m_buffer->packagefilecontent());
}

const boost::filesystem::path& CPluginInformation::getPath() const
{
   return boost::filesystem::path(m_buffer->path());
}
