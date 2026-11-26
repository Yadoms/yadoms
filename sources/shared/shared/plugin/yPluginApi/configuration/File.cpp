#include "stdafx.h"
#include "File.h"
#include <shared/encryption/Base64.h>
#include <shared/DataContainer.h>

using namespace shared::plugin::yPluginApi::configuration;

CFile::CFile()
   : _filesize(0)
{
}

void CFile::extractContent(CDataContainer& containerToFill) const
{
   containerToFill.set("name", getFileName());
   containerToFill.set("size", getSize());
   containerToFill.set("type", getMimeType());
   containerToFill.set("lastModified", getLastModificationDate().getBoostDateTime());
   containerToFill.set("content", _base64content);
}

void CFile::fillFromContent(const CDataContainer& initialData)
{
   _filename = initialData.get<std::string>("name");
   _filesize = initialData.get<unsigned int>("size");
   _mimeType = initialData.get<std::string>("type");
   const auto dateIso = initialData.get<std::string>("lastModified");
   auto a = boost::posix_time::from_iso_string(dateIso);
   _lastModified = dateTime::CDateTimeContainer(a);
   _base64content = initialData.get<std::string>("content");
}

void CFile::fillFromSerializedString(const std::string& serializedData)
{
   const CDataContainer deserializeData(serializedData);
   fillFromContent(deserializeData);
}

const std::string& CFile::getFileName() const
{
   return _filename;
}

unsigned int CFile::getSize() const
{
   return _filesize;
}

const std::string& CFile::getMimeType() const
{
   return _mimeType;
}

const shared::dateTime::CDateTimeContainer& CFile::getLastModificationDate() const
{
   return _lastModified;
}

const std::string& CFile::getContentAsBase64() const
{
   return _base64content;
}

std::string CFile::getContent() const
{
   return encryption::CBase64::decode(_base64content);
}
