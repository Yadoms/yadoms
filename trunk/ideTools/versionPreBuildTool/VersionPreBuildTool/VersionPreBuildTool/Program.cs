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
                                Console.WriteLine("Specified file not found : " + args[2]);
                            }

                        }
                        else
                        {
                            Console.WriteLine("Specified file not found : " + args[1]);
                        }

                    }
                    else
                    {
                        Console.WriteLine("Specified file not found : " + args[0]);
                    }

                }
                else
                {
                    Console.WriteLine("VersionPreBuildTool must be called with three parameters." + Environment.NewLine + "VersionPreBuildTool.exe {versionPath} {baseResPath} {generatedResPath}" + Environment.NewLine + "    versionPath : the path to the version.h file to read" + Environment.NewLine + "    baseResPath : the path to the base resource file" + Environment.NewLine + "    generatedResPath : the path to the resource file to generate");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("VersionPreBuildTool failed : " + ex.Message);
            }
        }

        static void ProcessVersion(string versionFile, string resFile, string generatedFile)
        {
            Version version;
            string releaseType;

            ReadVersion(versionFile, out version, out releaseType);
            WriteRes(resFile, version, releaseType, generatedFile);
        }

        static void ReadVersion(string versionFile, out Version version, out string releaseType)
        {
            Console.WriteLine("Reading yadoms information from " + versionFile);
            
            version = new Version();
            releaseType = "";
            
            string[] lines = File.ReadAllLines(versionFile);
            Regex regexVersion = new Regex("const std::string YadomsVersion\\(\\\"(\\d\\.\\d\\.\\d\\.\\d)\\\"\\)");
            Regex regexReleaseType = new Regex("const shared::versioning::EReleaseType YadomsReleaseType\\(shared::versioning::EReleaseType::k(.*)\\)");
            foreach (string line in lines)
            {
                Match matchVersion = regexVersion.Match(line);
                if (matchVersion.Success)
                {
                    version = Version.Parse(matchVersion.Groups[1].Value);
                }

                Match matchReleaseType = regexReleaseType.Match(line);
                if (matchReleaseType.Success)
                {
                    releaseType = matchReleaseType.Groups[1].Value;
                }
            }

            Console.WriteLine("    Version : " + version);
            Console.WriteLine("    ReleaseType : " + releaseType);

        }

        static void WriteRes(string resFile, Version version, string releaseType, string generatedFile)
        {
            string[] lines = File.ReadAllLines(resFile);
            for (int i = 0; i < lines.Length; ++i)
            {
                if (lines[i].StartsWith(" FILEVERSION") || lines[i].StartsWith(" PRODUCTVERSION"))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf(" ", StringComparison.Ordinal));
                    newLine += " " + version.Major + "," + version.Minor + "," + version.Build + "," + version.Revision + "";
                    lines[i] = newLine;
                } else if (lines[i].Contains("VALUE \"FileVersion\""))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf("FileVersion", StringComparison.Ordinal));
                    newLine += "FileVersion\", \"" + version.Major + "." + version.Minor + "." + version.Build + "." + version.Revision + "\"";
                    lines[i] = newLine;
                } else if (lines[i].Contains("VALUE \"ProductVersion\""))
                {
                    string newLine = lines[i].Remove(lines[i].LastIndexOf(",", StringComparison.Ordinal));
                    newLine += ", \"" + version.Major + "." + version.Minor + "." + version.Build + "." + version.Revision;
                    if (releaseType.ToLower() != "stable")
                        newLine += " " + releaseType;
                    newLine += "\"";
                    lines[i] = newLine;
                }
            }

            Console.WriteLine("Generating yadoms.rc file : " + generatedFile);
            File.WriteAllLines(generatedFile, lines);
        }
    }
}
