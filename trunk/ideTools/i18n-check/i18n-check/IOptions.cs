using System;
using System.Collections.Generic;

namespace i18n_check
{
    interface IOptions
    {
        bool Parse(string[] args);

        List<string> ExcludePaths { get; }

        List<string> ForcedLanguages { get; }
    }
}
