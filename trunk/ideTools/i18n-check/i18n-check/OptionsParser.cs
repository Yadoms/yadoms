using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommandLine;
using CommandLine.Text;

namespace i18n_check
{
    class OptionsParser
    {
        [OptionList('e', "exclude", Separator = ';', HelpText = "Specify paths to exclude (separated by ';').")]
        public List<string> RawExcludePaths { get; set; }

        [OptionList('l', "forcedLanguages", Separator = ';', HelpText = "Specify languages to check (by default, just check existing languages pour each modules).")]
        public List<string> ForcedLanguages { get; set; }

        [ParserState]
        public IParserState LastParserState { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this,
              (HelpText current) => HelpText.DefaultParsingErrorsHandler(this, current));
        }
    }
}
