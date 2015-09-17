using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace i18n_check
{
    class Options : IOptions
    {
        private OptionsParser _optionsParser = new OptionsParser();

        private List<string> _excludePaths;
        public List<string> ExcludePaths
        {
            get
            {
                if (_excludePaths != null)
                    return _excludePaths;

                _excludePaths = new List<string>();

                if (_optionsParser.RawExcludePaths == null)
                    return _excludePaths;

                foreach (var excludePath in _optionsParser.RawExcludePaths)
                    _excludePaths.Add(Path.GetFullPath(excludePath));

                return _excludePaths;
            }
        }

        public List<string> ForcedLanguages { get { return _optionsParser.ForcedLanguages; } }

        public bool Parse(string[] args)
        {
            return CommandLine.Parser.Default.ParseArguments(args, _optionsParser);
        }
    };
}
