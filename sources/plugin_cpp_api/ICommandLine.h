#pragma once


namespace plugin_cpp_api
{
	//--------------------------------------------------------------
	/// \class The command line parameter extraction interface
	//--------------------------------------------------------------
	class ICommandLine
	{
	public:
		virtual ~ICommandLine() {}

		//--------------------------------------------------------------
		/// \brief              Get the plugin execution context (Yadoms API)
		/// \return             Plugin execution context (Yadoms API)
		//--------------------------------------------------------------
		virtual std::string yPluginApiAccessorId() const = 0;
	};
} // namespace plugin_cpp_api
