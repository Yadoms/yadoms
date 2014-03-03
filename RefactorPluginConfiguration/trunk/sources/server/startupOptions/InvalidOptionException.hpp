//
// Custom validators for startup options loader
//
#pragma once


namespace startupOptions
{

   //--------------------------------------------------------------
   /// \class Invalid option exception
   // Exception thrown if there is an invalid value given
   // Catched as po::invalid_option_value in CLoader::ctor
   // This class add some information in error display
   //--------------------------------------------------------------
   class CInvalidOptionException : public boost::program_options::invalid_option_value
   {
   public:
      CInvalidOptionException(const std::string& value, const std::string& message)
         :invalid_option_value(value + " : " + message)
      {}
   };

} // namespace startupOptions