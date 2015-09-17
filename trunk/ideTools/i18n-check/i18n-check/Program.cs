using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace i18n_check
{
    class Program
    {
        private static IOptions _options = new Options();
        private static IStatistics _stat = new Statistics(_options);
        private static List<IModule> _modules = new List<IModule>();

        static void Main(string[] args)
        {
            try
            {
                if (!_options.Parse(args))
                    return;

                FindModules(Directory.GetCurrentDirectory());
                _stat.ModulesCount = _modules.Count;

                if (_modules.Count == 0)
                    throw new Exception("No module found");

                ProcessModules(_modules);

                Console.WriteLine(_stat);
            }
            catch(Exception ex)
            {
                Console.WriteLine("Error : " + ex.Message);
            }
        }

        private static void ProcessModules(List<IModule> modules)
        {
            var languages = new List<LanguageFile>();

            foreach(var module in modules)
                languages.AddRange(FindLanguages(module));

            if (languages.Count == 0)
            {
                Console.WriteLine("No language found");
                return;
            }

            foreach (var language in languages)
                Check(language);
        }

        private static void FindModules(string baseDirectory)
        {
            try
            {
                WalkDirectory(baseDirectory, directory =>
                {
                    var module = new Module(directory);
                    if (module.Valid)
                        _modules.Add(module);
                });
            }
            catch (System.Exception excpt)
            {
                throw new Exception("Unable to find modules : " + excpt.Message);
            }
        }

        static void WalkDirectory(string baseDirectory, Action<string> action)
        {
            action(baseDirectory);

            foreach (var directory in Directory.GetDirectories(baseDirectory))
            {
                if (_options.ExcludePaths == null || !_options.ExcludePaths.Contains(directory))
                    WalkDirectory(directory, action);                
            }
        }

        private static List<LanguageFile> FindLanguages(IModule module)
        {
            var languages = new List<LanguageFile>();

            if (_options.ForcedLanguages != null && _options.ForcedLanguages.Count != 0)
                module.ReplaceTranslations(_options.ForcedLanguages);

            foreach (var translation in module.Translations)
                languages.Add(new LanguageFile(module, translation, _options, _stat));

            return languages;
        }

        private static void Check(ILanguageFile language)
        {
            if (language.Check())
                _stat.CheckedLanguagesCount++;
            else
                _stat.MissingLanguages++;
        }
    }
}
