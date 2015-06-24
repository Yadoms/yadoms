/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function UpdateInformation(name, author, description, releaseType, url, credits, version, downloadUrl, iconUrl) {
   assert(!isNullOrUndefined(name), "name of a plugin must be defined");
   assert(!isNullOrUndefined(author), "author of a plugin must be defined");
   assert(!isNullOrUndefined(description), "description of plugin");
   assert(!isNullOrUndefined(releaseType), "release type must be defined");
   assert(!isNullOrUndefined(version), "version must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");
   assert(!isNullOrUndefined(iconUrl), "iconUrl must be defined");

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