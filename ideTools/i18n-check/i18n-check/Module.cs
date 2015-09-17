using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace i18n_check
{
    class Module : IModule
    {
        private string _directory;
        private string _refFileName = "en.json";
        private List<string> _translations = new List<string>();

        public Module(string directory)
        {
            _directory = directory;

            foreach(var translation in System.IO.Directory.GetFiles(_directory, "*.json"))
            {
                if (Path.GetFileName(translation) != _refFileName)
                    _translations.Add(Path.GetFileNameWithoutExtension(translation));
            }
        }

        public string Directory { get { return _directory; } }

        public string FullPathName { get { return Path.Combine(_directory, _refFileName); } }

        public bool Valid { get { return Path.GetFileName(_directory) == "locales" && System.IO.Directory.GetFiles(_directory, _refFileName).Count() == 1; } }

        public string[] Translations { get { return _translations.ToArray(); } }

        public void ReplaceTranslations(List<string> translations)
        {
            _translations = translations;
        }
    }
}
