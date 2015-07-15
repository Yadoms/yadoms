/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function UpdateInformation(type, name, author, description, releaseType, url, credits, version, downloadUrl, iconUrl) {
   assert(!isNullOrUndefined(type), "type of a plugin must be defined");
   assert(!isNullOrUndefined(name), "name of a plugin must be defined");
   assert(!isNullOrUndefined(author), "author of a plugin must be defined");
   assert(!isNullOrUndefined(description), "description of plugin");
   assert(!isNullOrUndefined(releaseType), "release type must be defined");
   assert(!isNullOrUndefined(version), "version must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");

   this.type = type;
   this.name = name;
   this.author = author;
   this.description = description;
   this.releaseType = releaseType;
   this.url = url;
   this.credits = credits;
   this.version = version;
   this.downloadUrl = downloadUrl;
   this.iconUrl = iconUrl;
}