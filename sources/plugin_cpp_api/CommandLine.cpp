#include "stdafx.h"
#include "CommandLine.h"


namespace plugin_cpp_api
{
	CCommandLine::CCommandLine(int argc, char** argv)
	{
		static const auto NbExpectedArguments = 2;

		if (argc != NbExpectedArguments)
			throw std::invalid_argument(std::string("Invalid arguments number (")
										+ std::to_string(argc)
										+ "), "
										+ std::to_string(NbExpectedArguments)
										+ " expected");

		m_yPluginApiAccessorId = argv[1];
	}

	CCommandLine::~CCommandLine()
	{
	}

	std::string CCommandLine::yPluginApiAccessorId() const
	{
		return m_yPluginApiAccessorId;
	}

} // namespace plugin_cpp_api
