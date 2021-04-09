#pragma once


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot HTTP(s) requests
      //--------------------------------------------------------------
      class CHttpHelpers final
      {
      public:
         CHttpHelpers() = delete;
         ~CHttpHelpers() = default;

         //--------------------------------------------------------------
         /// \brief	    Encode an URL (escape some characters)
         /// \param[in]  url                 input URL
         /// \return     Encoded URL
         //--------------------------------------------------------------
         static std::string urlEncode(const std::string& url);

         //--------------------------------------------------------------
         /// \brief	    Decode an URL (unescape some characters)
         /// \param[in]  url                 input URL
         /// \return     Decoded URL
         //--------------------------------------------------------------
         static std::string urlDecode(const std::string& url);
      };
   }
} // namespace shared::http
