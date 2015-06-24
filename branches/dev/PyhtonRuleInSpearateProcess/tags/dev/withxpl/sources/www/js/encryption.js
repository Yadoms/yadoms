/**
 * This file contains some methods for encrypting/decrypting strings
 */
 
  

/**
 * Encrypt or decrypt a string using XOR algorithm
 * @param stringToEncrypt 	The string to encrypt or decrypt
 * @param key 				The encryption/decryption key
 * @returns The encrypted/decrypted string
 */
function xorEncryptDecrypt(stringToEncrypt, key){
	var output = "";
	for (var i = 0; i < stringToEncrypt.length; i++)
		output+= String.fromCharCode(stringToEncrypt.charCodeAt(i) ^ key.charCodeAt(i % key.length));
	return output;
}

/**
 * Encrypt a string using XOR algorithm, with a base64 pass
 * @param toEncrypt  The string to encrypt
 * @param key        The encryption key
 * @return	The encrypted string (base64)
 */
function encryptBase64(toEncrypt, key) {
	var cypher = xorEncryptDecrypt(toEncrypt, key);
	return btoa(cypher);
}			

/**
 * Decrypt a base64 string using XOR algorithm
 * @param toDecrypt  The base64 string to decrypt
 * @param key        The decryption key
 * @return	The decrypted string
 */
function decryptBase64(toDecrypt, key) {
	return xorEncryptDecrypt(atob(toDecrypt), key);
}
