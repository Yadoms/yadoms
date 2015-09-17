using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace i18n_check
{
    class Statistics : IStatistics
    {
        public int ModulesCount { get; set; }

        public int TotalTokenCount { get; set; }

        public int NotTranslatedTokenCount { get; set; }

        public int MissingLanguages { get; set; }

        public int CheckedLanguagesCount { get; set; }
        
        public int ObsoleteTokenCount { get; set; }

        public int TranslatedTokenCount { get { return TotalTokenCount - NotTranslatedTokenCount; } }

        public int TranslatedRatio { get { return (int)((double)TranslatedTokenCount / (double)TotalTokenCount * 100); } }

        private IOptions _options;

        public Statistics(IOptions options)
        {
            _options = options;
        }

        public override string ToString()
        {
            string stat = Environment.NewLine;

            if (ModulesCount == 0)
            {
                stat += "No module found, nothing to do" + Environment.NewLine;
                return stat;
            }

            stat += String.Format("        {0} module(s) found", ModulesCount) + Environment.NewLine;

            stat += String.Format("        {0} language file(s) checked", CheckedLanguagesCount) + Environment.NewLine;

            if (MissingLanguages == 0 && NotTranslatedTokenCount == 0)
            {
                stat += "  Success : all labels are translated" + Environment.NewLine;
            }
            else
            {
                if (MissingLanguages != 0)
                    stat += String.Format("  Error : {0} translation file(s) missing or erroneous", MissingLanguages) + Environment.NewLine;

                if (NotTranslatedTokenCount != 0)
                    stat += String.Format("  Warning : {0}% ({1}/{2}) labels translated",
                        TranslatedRatio,
                        TranslatedTokenCount,
                        TotalTokenCount) + Environment.NewLine;
            }

            if (ObsoleteTokenCount > 0)
                stat += String.Format("  Warning : {0} obsolete translation(s) (should be deleted in translation files)", ObsoleteTokenCount) + Environment.NewLine;

            return stat;
        }
    }
}
