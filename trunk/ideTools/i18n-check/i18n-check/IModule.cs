using System;
using System.Collections.Generic;
namespace i18n_check
{
    interface IModule
    {
        string Directory { get; }
        string FullPathName { get; }
        bool Valid { get; }
        string[] Translations { get; }
        void ReplaceTranslations(List<string> translations);
    }
}
