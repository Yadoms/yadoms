using System;
using System.IO;
using System.Text.RegularExpressions;

namespace VersionPreBuildTool
{
    static class Program
    {
        /// <summary>
        /// Point d'entrée principal de l'application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            try
            {
                if (args.Length == 3)
                {
                    if (File.Exists(args[0]))
                    {
                        if (File.Exists(args[1]))
                        {
                            if (!string.IsNullOrEmpty(args[2]))
                            {
                                ProcessVersion(args[0], args[1], args[2]);
                            }
                            else
                            {
                                Console.WriteLine(@"Specified file not found : " + args[2]);
                            }

                        }
                        else
                        {
                            Console.WriteLine(@"Specified file not found : " + args[1]);
                        }

                    }
                    else
                    {
                        Console.WriteLine(@"Specified file not found : " + args[0]);
                    }

                }
                else
                {
                    Console.WriteLine(@"VersionPreBuildTool must be called with three parameters." + Environment.NewLine + @"VersionPreBuildTool.exe {versionPath} {baseResPath} {generatedResPath}" + Environment.NewLine + @"    versionPath : the path to the version.h file to read" + Environment.NewLine + @"    baseResPath : the path to the base resource file" + Environment.NewLine + @"    generatedResPath : the path to the resource file to generate");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(@"VersionPreBuildTool failed : " + ex.Message);
            }
        }

        static void ProcessVersion(string versionFile, string resFile, string generatedFile)
        {
            Semver version;

            ReadVersion(versionFile, out version);
            WriteRes(resFile, version, generatedFile);
        }

        static void ReadVersion(string versionFile, out Semver version)
        {
            Console.WriteLine(@"Reading yadoms information from " + versionFile);

            version = null;
            string[] lines = File.ReadAllLines(versionFile);
            Regex regexVersion = new Regex("const shared::versioning::CVersion YadomsVersion\\(\\\"(.*)\\\"\\)");

            foreach (string line in lines)
            {
                Match matchVersion = regexVersion.Match(line);
                if (matchVersion.Success)
                {
                    version = Semver.Parse(matchVersion.Groups[1].Value);
                }
            }

            Console.WriteLine(@"    Version : " + version);

        }

        static void WriteRes(string resFile, Semver version, string generatedFile)
        {
            string[] lines = File.ReadAllLines(resFile);
            for (int i = 0; i < lines.Length; ++i)
            {
                if (lines[i].StartsWith(" FILEVERSION") || lines[i].StartsWith(" PRODUCTVERSION"))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf(" ", StringComparison.Ordinal));
                    newLine += " " + version.ToCustomizedString(",",false, "", false, "") + ""; //with ","
                    lines[i] = newLine;
                } else if (lines[i].Contains("VALUE \"FileVersion\""))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf("FileVersion", StringComparison.Ordinal));
                    newLine += "FileVersion\", \"" + version + "\"";
                    lines[i] = newLine;
                } else if (lines[i].Contains("VALUE \"ProductVersion\""))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf(",", StringComparison.Ordinal));
                    newLine += ", \"" + version + "\"";
                    lines[i] = newLine;
                }
            }

            Console.WriteLine(@"Generating yadoms.rc file : " + generatedFile);
            File.WriteAllLines(generatedFile, lines);
        }
    }
}
