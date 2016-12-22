/** @module Plugin class */

/**
 * Creates an instance of Plugin
 * @constructor
 */
function UpdateInformation(type, name, author, description, url, credits, version, downloadUrl, iconUrl) {
   assert(!isNullOrUndefined(type), "type of a plugin must be defined");
   assert(!isNullOrUndefined(name), "name of a plugin must be defined");
   assert(!isNullOrUndefined(author), "author of a plugin must be defined");
   assert(!isNullOrUndefined(description), "description of plugin");
   assert(!isNullOrUndefined(version), "version must be defined");
   assert(!isNullOrUndefined(downloadUrl), "downloadUrl must be defined");

   this.type = type;
   this.name = name;
   this.author = author;
   this.description = description;
   this.url = url;
   this.credits = credits;
   this.version = new Version(version);
   this.downloadUrl = downloadUrl;
   this.iconUrl = iconUrl;
}