#pragma once
#include <string>


class IMailBodyBuilder
{
public:
   virtual ~IMailBodyBuilder() = default;

   /// @brief Set message subject
   /// @param subject Message subject
   /// @return Current object to chain calls
   virtual IMailBodyBuilder& setSubject(const std::string& subject) = 0;

   /// @brief Set body in Ascii format (optional)
   /// @param body Ascii message
   /// @note It's a good idea to use both Ascii and Html formats, as some client doesn't support Html format
   /// @return Current object to chain calls
   virtual IMailBodyBuilder& setAsciiBody(const std::string& body) = 0;

   /// @brief Set body in Html format (optional)
   /// @param body Html message
   /// @note It's a good idea to use both Ascii and Html formats, as some client doesn't support Html format
   /// @return Current object to chain calls
   virtual IMailBodyBuilder& setHtmlBody(const std::string& body) = 0;

   /// @brief Set carbon copy recipients list
   /// @param ccRecipients The recipients list
   /// @return Current object to chain calls
   virtual IMailBodyBuilder& setCarbonCopyRecipients(std::vector<std::string> ccRecipients) = 0;

   /// @brief Build the email body
   /// @return The built email
   virtual std::string build() = 0;
};
