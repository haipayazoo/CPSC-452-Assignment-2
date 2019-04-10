#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

//enums for the cipher and mode arguments
enum cipher {aes, des};
enum mode {encrypt, decrypt};

bool parseArguments(int argc,
					char** argv,
					enum cipher *argCipher,
					unsigned char **argKey,
					enum mode *argMode,
					unsigned char **argInputFile,
					unsigned char **argOutputFile)
{
	//check number of arguments
	if(argc != 6)
	{
		printf("Incorrect number of arguments\n");
		return false;
	}

	//parse the cipher
	if(strcmp(argv[1], "AES") == 0)
	{
		*argCipher = aes;
	}
	else if(strcmp(argv[1], "DES") == 0)
	{
		*argCipher = des;
	}
	else
	{
		printf("Incorrect cipher [%s]\n", argv[1]);
		return false;
	}

	//parse the key
	*argKey = (unsigned char*)argv[2];

	// TODO: validate key length (64 bits for des, 128 bits for aes)

	//parse the mode
	if(strcmp(argv[3], "ENC") == 0)
	{
		*argMode = encrypt;
	}
	else if(strcmp(argv[3], "DEC") == 0)
	{
		*argMode = decrypt;
	}
	else
	{
		printf("Incorrect mode [%s]\n", argv[3]);
		return false;
	}

	//parse the input file
	*argInputFile = (unsigned char*)argv[4];

	//parse the output file
	*argOutputFile = (unsigned char*)argv[5];

	return true;
}

int main(int argc, char** argv)
{
	/**
	 * TODO: Replace the code below	with your code which can SWITCH
	 * between DES and AES and encrypt files. DO NOT FORGET TO PAD
	 * THE LAST BLOCK IF NECESSARY.
	 *
	 * NOTE: due to the incomplete skeleton, the code may crash or
	 * misbehave.
	 */

	/* Create an instance of the DES cipher */	
	CipherInterface* cipher = NULL; 

	//create variables for the cipher and mode
	enum cipher argCipher;
	unsigned char *argKey;
	enum mode argMode;
	unsigned char *argInputFile;
	unsigned char *argOutputFile;

	//parse arguments
	if(!parseArguments(argc, argv, &argCipher, &argKey, &argMode, &argInputFile, &argOutputFile))
	{
		printf("Unable to parse arguments\n");
		return 0;
	}

	//initialize the cipher
	if(argCipher == aes)
	{
		cipher = new AES();
	}
	else if(argCipher == des)
	{
		cipher = new DES();
	}

	/* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}

	/* Set the encryption key
	 * A valid key comprises 16 hexidecimal
	 * characters. Below is one example.
	 * Your program should take input from
	 * command line.
	 */
	cipher->setKey(argKey);
	
	/* Perform encryption */
	unsigned char* cipherText = cipher->encrypt((unsigned char*)"hello world");

	printf("CT: %s\n", cipherText);
	
	/* Perform decryption */
	unsigned char* plainText = cipher->decrypt(cipherText);	

	printf("PT: %s\n", plainText);
	
	return 0;
}
