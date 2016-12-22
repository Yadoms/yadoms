/** @module Version class 
 * Following Semver 2.0.0
*/

/**
 * Creates an instance of Version
 * @constructor
 */
function Version(versionAsText) {
  assert(!isNullOrUndefined(versionAsText), "versionAsText must be defined");

  var match = Version.regex.exec(versionAsText);
  if ((!match) || match.length < 4) 
    throw Error("Unable to parse semver version: " + versionAsText);
    
  console.log(match[1]); // abc
  this.major_ = match[1];
  this.minor_ = match[2];
  this.patch_ = match[3];

  if (match.length >= 5)
    this.preRelease_ = match[4];

  if (match.length >= 6)
    this.buildMetadata_ = match[5];

   this.versionAsText_ = versionAsText;
}

//Regex for Semver
//Taken from http://rgxdb.com/
Version.regex = "/(?<=^[Vv]|^)(?:(?<major>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<minor>(?:0|[1-9](?:(?:0|[1-9])+)*))[.](?<patch>(?:0|[1-9](?:(?:0|[1-9])+)*))(?:-(?<prerelease>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*)))*))?(?:[+](?<build>(?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+)))*))?)$/";

Version.prototype.toJSON = function () {
   return {
      version: this.versionAsText_
   };
};

/**
 * Get the version as string format 
*/
Version.prototype.toString = function() {
  return this.versionAsText_;
};

/**
 * Compare the two UpdateInformationObjects. Return < 0 if item1 is lower, =0 if the same and >0 if item1 is higher
  */
Version.compare = function(first, second) {
  //Comparison (matching to SEMVER requirements)
  // compare major, minor and patch
  // if the same, the preRelease field (alphabetically ordered) make the precendence

  if (first.major_ > second.major_)
      return 1;
  else if (first.major_ < second.major_)
      return -1;
  else {
    if (first.minor_ > second.minor_)
      return 1;
    else if (first.minor_ < second.minor_)
      return -1;
    else {
      if (first.patch_ > second.patch_)
          return 1;
      else if (first.patch_ < second.patch_)
          return -1;
      else {
        //if both not defined version is equal
        if (!first.preRelease_ && !second.preRelease_)
          return 0;
        else {
          if (!first.preRelease_ && second.preRelease_)
              return 1;
          if (first.preRelease_ && !second.preRelease_)
              return -1;
          return first.preRelease_.localeCompare(second.preRelease_);
        }
      }               
    }
  }
}