/** @module Version class 
 * Following Semver 2.0.0
*/

/**
 * Creates an instance of Version
 * @constructor
 */
function Version(versionAsText) {
  var match = Version.regex.exec(versionAsText || '0.0.0');
  if ((!match) || match.length < 4) 
    throw Error("Unable to parse semver version: " + versionAsText);
    
  this.major_ = match[1];
  this.minor_ = match[2];
  this.patch_ = match[3];

  if (match.length >= 5)
    this.preRelease_ = match[4];

  if (match.length >= 6)
    this.buildMetadata_ = match[5];

   this.versionAsText_ = versionAsText || '0.0.0';
}

//Regex for Semver
//Taken from http://rgxdb.com/r/40OZ1HN5 and ajusted to remove named groups because unsupported in vanillaJS
Version.regex = /^[vV]{0,1}(?:((?:0|[1-9](?:(?:0|[1-9])+)*))[.]((?:0|[1-9](?:(?:0|[1-9])+)*))[.]((?:0|[1-9](?:(?:0|[1-9])+)*))(?:-((?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:0|[1-9](?:(?:0|[1-9])+)*)))*))?(?:[+]((?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+))(?:[.](?:(?:(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?|(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)(?:[A-Za-z]|-)(?:(?:(?:0|[1-9])|(?:[A-Za-z]|-))+)?)|(?:(?:0|[1-9])+)))*))?)$/;

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
 * Get the major version number
*/
Version.prototype.major = function() {
   return this.major_;
}

/**
 * Get the minor version number
*/
Version.prototype.minor = function() {
   return this.minor_;
}

/**
 * Get the patch version number
*/
Version.prototype.patch = function() {
   return this.patch_;
}

/**
 * Get the prerelease version (can be null or empty)
*/
Version.prototype.preRelease = function() {
   return this.preRelease_;
}

/**
 * Get the build metadata version (can be null or empty)
*/
Version.prototype.buildMetadata = function() {
   return this.buildMetadata_;
}


/**
 * Compare the two UpdateInformationObjects. Return < 0 if item1 is lower, =0 if the same and >0 if item1 is higher
  */
Version.compare = function(A, B) {
  //Comparison (matching to SEMVER requirements)
  // compare major, minor and patch
  // if the same, the preRelease field (alphabetically ordered) make the precendence
  var first = A;
  if (typeof A === "string" || !A) {
     first = new Version(A);
  }
  var second = B;
  if (typeof B === "string" || !B) {
     second = new Version(B);
  }
  
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