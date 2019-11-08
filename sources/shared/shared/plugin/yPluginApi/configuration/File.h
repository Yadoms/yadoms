#pragma once
#include <shared/IDataContainable.h>
#include <shared/dateTime/DateTimeContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace configuration
         {
            //-----------------------------------------------------
            ///\brief A container for file data
            //-----------------------------------------------------
            class CFile : IDataContainable
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               //-----------------------------------------------------
               explicit CFile();

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CFile();

               //[START] IDataContainable implementation
               void extractContent(CDataContainer & containerToFill) const override;
               void fillFromContent(const CDataContainer & initialData) override;
               void fillFromSerializedString(const std::string & serializedData) override;
               //[END] IDataContainable implementation

               //-----------------------------------------------------
               ///\brief  Obtain the filename
               //-----------------------------------------------------
               const std::string & getFileName() const;

               //-----------------------------------------------------
               ///\brief  Obtain the file size in bytes
               //-----------------------------------------------------
               unsigned int getSize() const;

               //-----------------------------------------------------
               ///\brief  Obtain the mime type
               //-----------------------------------------------------
               const std::string & getMimeType() const;

               //-----------------------------------------------------
               ///\brief  Get the last modification date
               //-----------------------------------------------------
               const dateTime::CDateTimeContainer & getLastModificationDate() const;

               //-----------------------------------------------------
               ///\brief  Get the file content (base64)
               //-----------------------------------------------------
               const std::string & getContentAsBase64() const;

               //-----------------------------------------------------
               ///\brief  Get the file content (raw, base 64 decoded)
               //-----------------------------------------------------
               std::string getContent() const;

            private:
               //-----------------------------------------------------
               ///\brief  The file name (no path provided, only filename with extension)
               //-----------------------------------------------------
               std::string m_filename;

               //-----------------------------------------------------
               ///\brief  The file size in bytes
               //-----------------------------------------------------
               unsigned int m_filesize;

               //-----------------------------------------------------
               ///\brief  The file mimeType
               //-----------------------------------------------------
               std::string m_mimeType;

               //-----------------------------------------------------
               ///\brief  The last modification date
               //-----------------------------------------------------
               dateTime::CDateTimeContainer m_lastModified;

               //-----------------------------------------------------
               ///\brief  The content as base64
               //-----------------------------------------------------
               std::string m_base64content;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::configuration


