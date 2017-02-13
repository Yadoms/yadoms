#pragma once


namespace interpreter_cpp_api
{
	//--------------------------------------------------------------
	/// \class The command line parameter extraction interface
	//--------------------------------------------------------------
	class ICommandLine
	{
	public:
		virtual ~ICommandLine() {}

		//--------------------------------------------------------------
		/// \brief              Get the interpreter execution context (Yadoms API)
		/// \return             Interpreter execution context (Yadoms API)
		//--------------------------------------------------------------
		virtual std::string yInterpreterApiAccessorId() const = 0;
	};
} // namespace interpreter_cpp_api
