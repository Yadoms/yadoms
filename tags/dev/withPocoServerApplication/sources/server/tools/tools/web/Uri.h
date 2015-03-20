#pragma once

namespace tools { namespace web { 

   //--------------------------------------------
   ///\brief Class used to validate and parse URI
   //--------------------------------------------
   class CUri
   {
   public:
      //--------------------------------------------
      ///\brief Constructor. Validate and parse an url
      ///\param [in] uri : the uri to validate and parse
      ///\throw   shared::exception::CException if URI is not valid
      //--------------------------------------------
      CUri(const std::string & uri);

      //--------------------------------------------
      ///\brief Destructor
      //--------------------------------------------
      virtual ~CUri();

      //--------------------------------------------
      ///\brief Determine if the url used in constructor us valid
      ///\return true if the uri is valid
      //--------------------------------------------
      bool isValid() const;

      //--------------------------------------------
      ///\brief   Get the schema of URI
      ///\return  the uri schema
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : "http"
      //--------------------------------------------
      const std::string & getUriSchema() const;

      //--------------------------------------------
      ///\brief   Get the domain of URI
      ///\return  the domain of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : website.com
      //--------------------------------------------
      const std::string & getUriDomain() const;

      //--------------------------------------------
      ///\brief   Get the port of URI (if specified in URI)
      ///\return  the port of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : 8080
      ///      input uri : http://www.website.com/path/to/download/package.zip
      //       return : ""
      //--------------------------------------------
      const std::string & getUriPort() const;

      //--------------------------------------------
      ///\brief   Get the path of URI
      ///\return  the path of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : /path/to/download/
      //--------------------------------------------
      const std::string & getUriPath() const;

      //--------------------------------------------
      ///\brief   Get the file of URI
      ///\return  the file of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : package.zip
      //--------------------------------------------
      const std::string & getUriFile() const;

      //--------------------------------------------
      ///\brief   Get the query string of URI
      ///\return  the query string of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : 
      //--------------------------------------------
      const std::string & getUriQueryString() const;

      //--------------------------------------------
      ///\brief   Get the hash of URI
      ///\return  the hash of URI
      ///\example : 
      ///      input uri : http://www.website.com:8080/path/to/download/package.zip
      //       return : 
      //--------------------------------------------
      const std::string & getUriHash() const;
   
   private:
      //--------------------------------------------
      ///\brief enumeration for result order
      //--------------------------------------------
      enum EResultFieldOrder
      {
         kSchema = 2,
         kDomain = 3, 
         kPort = 5, 
         kPath = 6, 
         kFile = 8, 
         kQuerystring = 9, 
         kHash = 12
      };

      const std::string & getResult(EResultFieldOrder field) const;

      //--------------------------------------------
      ///\brief Container for all URI parts
      //--------------------------------------------
      std::vector<std::string> m_resultFields;

      //--------------------------------------------
      ///\brief Indicate if the URI is valid
      //--------------------------------------------
      bool m_isValid;
   };

} //namespace web
} //namespace tools
