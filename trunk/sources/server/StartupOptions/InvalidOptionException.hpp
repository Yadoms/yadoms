//
// Custom validators for startup options loader
//
#pragma once

namespace po = boost::program_options;


//--------------------------------------------------------------
/// \class Invalid option exception
// Exception thrown if there is an invalid value given
// Catched as po::invalid_option_value in CStartupOptionsLoader::ctor
// This class add some information in error display
//--------------------------------------------------------------
class CInvalidOptionException : public po::invalid_option_value
{
public:
   CInvalidOptionException(const std::string& value, const std::string& message)
      :invalid_option_value(value + " : " + message)
   {}
};
