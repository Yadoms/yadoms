#pragma once
#include "ICommandLine.h"

namespace interpreter_cpp_api
{
	//--------------------------------------------------------------
	/// \class The command line parameter extraction
	//--------------------------------------------------------------
	class CCommandLine : public ICommandLine
	{
	public:
		CCommandLine(int argc, char** argv);

		virtual ~CCommandLine();

		std::string yInterpreterApiAccessorId() const override;

	private:
		
		std::string m_yInterpreterApiAccessorId;
	};
} // namespace interpreter_cpp_api
