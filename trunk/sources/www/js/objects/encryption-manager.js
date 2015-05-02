/**
 * This file contains some methods for encrypting/decrypting strings
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function EncryptionManager(){}

//not possible to have a strong password in js so ...
EncryptionManager.key = "2m72fgEQ";

/**
 * Encrypt or decrypt a string using XOR algorithm
 * @param stringToEncrypt 	The string to encrypt or decrypt
 * @param key 				The encryption/decryption key
 * @returns {string} The encrypted/decrypted string
 */
EncryptionManager.xorEncryptDecrypt = function(stringToEncrypt, key) {
    var output = "";
	for (var i = 0; i < stringToEncrypt.length; i++)
		output+= String.fromCharCode(stringToEncrypt.charCodeAt(i) ^ key.charCodeAt(i % key.length));
	return output;
};

/**
 * Encrypt a string using XOR algorithm, with a base64 pass
 * @param toEncrypt  The string to encrypt
 * @param key        The encryption key
 * @return	{string} The encrypted string (base64)
 */
EncryptionManager.encryptBase64 = function(toEncrypt, key) {
	if (isNullOrUndefined(toEncrypt))
		toEncrypt = "";
	var cypher = EncryptionManager.xorEncryptDecrypt(toEncrypt, key);
	return btoa(cypher);
};

/**
 * Decrypt a base64 string using XOR algorithm
 * @param toDecrypt  The base64 string to decrypt
 * @param key        The decryption key
 * @return	{string} The decrypted string
 */
EncryptionManager.decryptBase64 = function(toDecrypt, key) {
	if (isNullOrUndefined(toDecrypt))
		toDecrypt = "";
	return EncryptionManager.xorEncryptDecrypt(atob(toDecrypt), key);
};
