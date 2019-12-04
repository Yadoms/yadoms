#include "stdafx.h"
#include "Helpers.h"
#include "tooLowRevisionException.hpp"

namespace equipments
{
	void checkRevision(const std::string& revision) {
		//separation of letters and digits
		boost::regex reg("V(\\d+)(\\.)?(\\d+\\.)?(\\*|\\d+)([A-Z])");
		boost::smatch match;

		//Check the version
		if (boost::regex_search(revision, match, reg)) {
			// match[4] => 83 The sub-number revision
			// match[5] => E The revision letter
			if ((boost::lexical_cast<int>(match[4]) < 83) || ((boost::lexical_cast<int>(match[4]) == 83) && (match[5] < 'H')))
				throw CtooLowRevisionException("WES revision is < 0.83H");
		}
		else
			throw std::runtime_error("Could not check WES revision");
	}
}// namespace equipments