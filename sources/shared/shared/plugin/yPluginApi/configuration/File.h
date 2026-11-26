#pragma once
#include <shared/IDataContainable.h>
#include <shared/dateTime/DateTimeContainer.h>

namespace shared::plugin::yPluginApi::configuration
{
   //-----------------------------------------------------
   ///\brief A container for file data
   //-----------------------------------------------------
   class CFile final : IDataContainable
   {
   public:
      explicit CFile();

      CFile(const CFile&) = delete;
      CFile(CFile&&) = default;
      CFile& operator=(const CFile&) = delete;
      CFile& operator=(CFile&&) = delete;

      ~CFile() override = default;

      //[START] IDataContainable implementation
      void extractContent(CDataContainer& containerToFill) const override;
      void fillFromContent(const CDataContainer& initialData) override;
      void fillFromSerializedString(const std::string& serializedData) override;
      //[END] IDataContainable implementation

      //-----------------------------------------------------
      ///\brief  Obtain the filename
      //-----------------------------------------------------
      const std::string& getFileName() const;

      //-----------------------------------------------------
      ///\brief  Obtain the file size in bytes
      //-----------------------------------------------------
      unsigned int getSize() const;

      //-----------------------------------------------------
      ///\brief  Obtain the mime type
      //-----------------------------------------------------
      const std::string& getMimeType() const;

      //-----------------------------------------------------
      ///\brief  Get the last modification date
      //-----------------------------------------------------
      const dateTime::CDateTimeContainer& getLastModificationDate() const;

      //-----------------------------------------------------
      ///\brief  Get the file content (base64)
      //-----------------------------------------------------
      const std::string& getContentAsBase64() const;

      //-----------------------------------------------------
      ///\brief  Get the file content (raw, base 64 decoded)
      //-----------------------------------------------------
      std::string getContent() const;

   private:
      //-----------------------------------------------------
      ///\brief  The file name (no path provided, only filename with extension)
      //-----------------------------------------------------
      std::string _filename;

      //-----------------------------------------------------
      ///\brief  The file size in bytes
      //-----------------------------------------------------
      unsigned int _filesize;

      //-----------------------------------------------------
      ///\brief  The file mimeType
      //-----------------------------------------------------
      std::string _mimeType;

      //-----------------------------------------------------
      ///\brief  The last modification date
      //-----------------------------------------------------
      dateTime::CDateTimeContainer _lastModified;

      //-----------------------------------------------------
      ///\brief  The content as base64
      //-----------------------------------------------------
      std::string _base64content;
   };
}
