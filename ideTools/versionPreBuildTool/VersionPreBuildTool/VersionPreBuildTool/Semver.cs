using System;
using System.Text.RegularExpressions;

namespace VersionPreBuildTool
{
    internal class Semver : IComparable<Semver>, IEquatable<Semver>
    {

        public int Major { get; }
        public int Minor { get; }
        public int Patch { get; }
        public string PreRelease { get; }
        public string Build { get; }

        public Semver(string version)
        {
            Regex regexVersion = new Regex("(?<=^[Vv]|^)(?:(?<major>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<minor>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<patch>(?:0|[1-9](?:(?:0|[1-9])+)*))(?:-(?<prerelease>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*)))*))?(?:[+](?<build>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+)))*))?)$");
            Match matchVersion = regexVersion.Match(version);
            if (matchVersion.Success)
            {
                if (matchVersion.Groups.Count > 3)
                {
                    Major = Convert.ToInt32(matchVersion.Groups[1].Value);
                    Minor = Convert.ToInt32(matchVersion.Groups[2].Value);
                    Patch = Convert.ToInt32(matchVersion.Groups[3].Value);

                    if (matchVersion.Groups.Count > 4)
                    {
                        PreRelease = matchVersion.Groups[4].Value;

                        if (matchVersion.Groups.Count > 5)
                        {
                            Build = matchVersion.Groups[5].Value;
                        }
                    }
                }
                else
                {
                    throw new Exception("Fail to parse SEMVER syntax for " + version);
                }
            }
            else
            {
                throw new Exception("Invalid SEMVER syntax for " + version);
            }
        }
        public Semver(int major, int minor, int patch, string preRelease="", string build="")
        {
            Major = major;
            Minor = minor;
            Patch = patch;
            PreRelease = preRelease;
            Build = build;
        }

        public int CompareTo(Semver other)
        {
            // 0 : equal
            // 1 : this > other
            //-1 : this < other

            //Comparison (matching to SEMVER requirements)
            // compare major, minor and patch
            // if the same, the preRelease field (alphabetically ordered) make the precendence
            if (ReferenceEquals(other, null))
                return 1;
            if (Major > other.Major)
                return 1;
            if (Major < other.Major)
                return -1;
            if (Minor > other.Minor)
                return 1;
            if (Minor < other.Minor)
                return -1;
            if (Patch > other.Patch)
                return 1;
            if (Patch < other.Patch)
                return -1;
            if (PreRelease.Length==0 && other.PreRelease.Length == 0)
                return 0;
            if (PreRelease.Length == 0 && other.PreRelease.Length != 0)
                return 1;
            if (PreRelease.Length != 0 && other.PreRelease.Length == 0)
                return -1;
            return string.Compare(PreRelease, other.PreRelease, StringComparison.Ordinal);
        }

        public bool Equals(Semver other)
        {
            if (ReferenceEquals(other, null))
            {
                return false;
            }
            return CompareTo(other) == 0;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (ReferenceEquals(this, obj)) return true;
            return obj.GetType() == GetType() && Equals((Semver)obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = Major;
                hashCode = (hashCode * 397) ^ Minor;
                hashCode = (hashCode * 397) ^ Patch;
                hashCode = (hashCode * 397) ^ (PreRelease?.GetHashCode() ?? 0);
                hashCode = (hashCode * 397) ^ (Build?.GetHashCode() ?? 0);
                return hashCode;
            }
        }

        public static bool operator ==(Semver a, Semver b)
        {
            if (ReferenceEquals(a, null))
            {
                return ReferenceEquals(b, null);
            }
            return a.Equals(b);
        }

        public static bool operator !=(Semver a, Semver b)
        {
            return !(a == b);
        }

        public static bool operator >(Semver a, Semver b)
        {
            if (ReferenceEquals(a, null))
            {
                return false;
            }
            return a.CompareTo(b) > 0;
        }

        public static bool operator >=(Semver a, Semver b)
        {
            if (ReferenceEquals(a, null))
            {
                return ReferenceEquals(b, null);
            }
            return a.CompareTo(b) >= 0;
        }

        public static bool operator <(Semver a, Semver b)
        {
            if (ReferenceEquals(a, null))
            {
                return !ReferenceEquals(b, null);
            }
            return a.CompareTo(b) < 0;
        }

        public static bool operator <=(Semver a, Semver b)
        {
            if (ReferenceEquals(a, null))
            {
                return true;
            }
            return a.CompareTo(b) <= 0;
        }

        public override string ToString()
        {
            return ToCustomizedString();
        }

        public string ToCustomizedString(string digitSeprator=".", bool appendPrerelease = true, string preReleaseSeparator = "-", bool appendBuild=true, string buildSeparator="+")
        {
            string prerelease = PreRelease.Length == 0 || !appendPrerelease ? "" : preReleaseSeparator + PreRelease;
            string buildMetadata = Build.Length == 0 || !appendBuild ? "" : buildSeparator + Build;
            return $"{Major}{digitSeprator}{Minor}{digitSeprator}{Patch}{prerelease}{buildMetadata}";
        }
        public static Semver Parse(string version)
        {
            return new Semver(version);
        }
    }
}

