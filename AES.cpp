#include "AES.h"

/**
 * Sets the key to use
 * @param key - the first byte of this represents whether
 * to encrypt or to decrypt. 00 means encrypt and any other
 * value to decrypt.  Then come the bytes of the 128-bit key
 * (should be 16 of them).
 * @return - True if the key is valid and False otherwise
 */
bool AES::setKey(const unsigned char* keyArray)
{

	// AES implementation of openssl cares about whether
	// you are encrypting or decrypting when setting the key.
	// That is, when encrypting you use function AES_set_encrypt_key(...)
	// and when decrypting AES_set_decrypt_key(...).
	//
	// One way to solve this problem is to pass in a 17 byte key, where
	// the first byte is used to indicate whether we are encrypting or
	// decrypting. E.g., if the first byte is 0, then use AES_set_encrypt_key(...).
	// Otherwise, use AES_set_decrypt_key(...).  The rest of the bytes in the
	// array indicate the 16 bytes of the 128-bit AES key.
	//
	// Both functions return 0 on success and other values on faliure.
	// For documentation, please see https://boringssl.googlesource.com/boringssl/+/2623/include/openssl/aes.h
	// and aes.cpp example provided with the assignment.
	unsigned char* key = new unsigned char[16];

	// Copy key out of keyArray
	for (int i = 1; i < 17; i++)
	{
		key[i-1] = keyArray[i];
	}

	if (keyArray[0] == '0')
	{

		// Encrypting
		return AES_set_encrypt_key(key, 128, &this->enc_key);
	}
	else
	{

		// Decrypting
		return AES_set_decrypt_key(key, 128, &this->dec_key);
	}
}

/**
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
unsigned char* AES::encrypt(const unsigned char* plainText)
{

	//	1. Dynamically allocate a block to store the ciphertext.

	unsigned char* out = new unsigned char[17];

	//	2. Use AES_ecb_encrypt(...) to encrypt the text (please see the URL in setKey(...)
	//	and the aes.cpp example provided.

	// TODO THE PROBLEM RESIDES HERE. Getting segmentation dump error on this!
	AES_ecb_encrypt(plainText, out, &this->enc_key, AES_ENCRYPT);

	// 	3. Return the pointer to the ciphertext

	return out;
}

/**
 * Decrypts a string of ciphertext
 * @param cipherText - the ciphertext
 * @return - the plaintext
 */
unsigned char* AES::decrypt(const unsigned char* cipherText)
{

	//	1. Dynamically allocate a block to store the plaintext.

	unsigned char* out = new unsigned char[17];

	//	2. Use AES_ecb_encrypt(...) to decrypt the text (please see the URL in setKey(...)
	//	and the aes.cpp example provided.

	AES_ecb_encrypt(cipherText, out, &this->enc_key, AES_DECRYPT);

	// 	3. Return the pointer to the plaintext

	return out;

}
