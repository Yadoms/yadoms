using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace i18n_check
{
    class LanguageFile : ILanguageFile
    {
        private IModule _refModuleFile;
        private string _languageFile;
        private IOptions _options;
        private IStatistics _stat;
        private JToken _moduleRootNode;
        private JToken _translatedRootNode;

        public LanguageFile(IModule refModuleFile, string languageFile, IOptions options, IStatistics stat)
        {
            _refModuleFile = refModuleFile;
            _languageFile = languageFile;
            _options = options;
            _stat = stat;
        }

        public bool Check()
        {
            Console.WriteLine("Check module : " + _refModuleFile.Directory + ", langage " + _languageFile);

            var _languageFullPath = Path.ChangeExtension(Path.Combine(_refModuleFile.Directory, _languageFile), "json");
            if (!File.Exists(_languageFullPath))
            {
                Console.WriteLine(_refModuleFile.Directory + ": error : \"" + Path.GetFileName(_languageFullPath) + "\" not found");
                return false;
            }

            try
            {
                _moduleRootNode = JToken.Parse(File.ReadAllText(_refModuleFile.FullPathName));
                _translatedRootNode = JToken.Parse(File.ReadAllText(_languageFullPath));

                FindNotTranslatedTokens();

                FindObsoleteTranslations();
            }
            catch(Exception exception)
            {
                Console.WriteLine(_refModuleFile.Directory + ": error checking module : " + exception.Message);
                return false;
            }

            return true;
        }

        private void FindNotTranslatedTokens()
        {
            WalkStringNode(_moduleRootNode, node =>
            {
                _stat.TotalTokenCount++;

                var translatedToken = _translatedRootNode.SelectToken(node.Path);
                if (translatedToken == null)
                {
                    Console.WriteLine(_refModuleFile.Directory + ", " + _languageFile + ": error : \"" + node.Path + "\" not found");
                    _stat.NotTranslatedTokenCount++;
                }
            });
        }

        private void FindObsoleteTranslations()
        {
            WalkStringNode(_translatedRootNode, node =>
            {
                if (node.Type == JTokenType.String)
                {
                    var moduleToken = _moduleRootNode.SelectToken(node.Path);
                    if (moduleToken == null)
                    {
                        Console.WriteLine(_refModuleFile.Directory + ", " + _languageFile + ": warning : \"" + node.Path + "\" is obsolete");
                        _stat.ObsoleteTokenCount++;
                    }
                }
            });
        }

        static void WalkStringNode(JToken node, Action<JToken> action)
        {
            if (node.Type == JTokenType.String)
            {
                action(node);
            }
            else if (node.Type == JTokenType.Object)
            {

                foreach (JProperty child in node.Children<JProperty>())
                {
                    WalkStringNode(child.Value, action);
                }
            }
            else if (node.Type == JTokenType.Array)
            {
                foreach (JToken child in node.Children())
                {
                    WalkStringNode(child, action);
                }
            }
        }

        public string LanguageName { get { return Path.GetFileNameWithoutExtension(_languageFile); } }
    }
}
