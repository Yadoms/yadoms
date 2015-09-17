using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace i18n_check
{
    interface IStatistics
    {
        int ModulesCount { get; set; }

        int TotalTokenCount { get; set; }

        int NotTranslatedTokenCount { get; set; }

        int MissingLanguages { get; set; }

        int CheckedLanguagesCount { get; set; }

        int ObsoleteTokenCount { get; set; }
    }
}
