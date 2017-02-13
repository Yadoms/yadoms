#pragma once

//--------------------------------------------------------------
/// \class Exception for log Configuration exception
//--------------------------------------------------------------
class CLogConfigurationException : public std::exception
{
public:
  //--------------------------------------------------------------
  /// \brief	                        Constructor
  /// \param [in] message : Message to display
  //--------------------------------------------------------------
  explicit CLogConfigurationException(const std::string & message) : m_message(message)
  {
  }

  //--------------------------------------------------------------
  /// \brief      Destructor
  //--------------------------------------------------------------
  virtual ~CLogConfigurationException() throw()
  {
  }

  //--------------------------------------------------------------
  /// \brief	    Build full message explaining exception reason
  /// \return     message explaining exception reason
  //--------------------------------------------------------------
  virtual const char* what() const throw() { return m_message.c_str(); }

private:
  std::string m_message;
};