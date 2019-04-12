#include <string>
#include <fstream>
#include <vector>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

//enums for the cipher and mode arguments
enum cipher {aes = 0, des = 1};
enum mode {encrypt, decrypt};

//takes a pointer to an array of characters and modifies the array
//to pad the array at and after the last '\n' character
void pad_zero(unsigned char** input)
{
	//search the array from the back to the front to find the last instance of a '\n' character
	for(int i = 7; i >= 0; --i)
	{
		//if the character is a newline character
		if(((char*)input)[i] == '\n')
		{
			//set the character to the pad
			((char*)input)[i] = '0';
			//break the loop
			i = 0;
		}
		else//if the newline character has yet to be found
		{
			//set the pad
			((char*)input)[i] = '0';
		}
	}
}

//removes the zero padding from a string
void pad_zero_remove(unsigned char** input)
{
	//find the end of the array
	int endIndex = 0;
	while(((char*)input)[endIndex] != '\0')
	{
		endIndex++;
	}

	//loop from the back to the front and eliminate all the '0' characters
	//start at i=1 to ignore the last '\0' null terminator
	for(int i = 1; i < 8; i++)
	{
		if(((char*)input)[endIndex - i] != '0')
		{
			//break the loop
			i = 8;
		}
		else
		{
			//remove the character from the array
			((char*)input)[endIndex - i] = '\0';
		}
	}
}

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

	//parse the key
	//if the cipher is aes then we need to add a '0' or '1' onto the front of the key
	//to indicate encryption or decryption
	if(*argCipher == aes)
	{
		if(*argMode == encrypt)
		{
			*argKey = (unsigned char*)realloc(*argKey, sizeof(unsigned char) * 17);
			strcpy((char*)*argKey, "0");
			strcat((char*)*argKey, argv[2]);
		}
		else
		{
			*argKey = (unsigned char*)realloc(*argKey, sizeof(unsigned char) * 17);
			strcpy((char*)*argKey, "1");
			strcat((char*)*argKey, argv[2]);
		}
	}
	else
	{
		//if its des just set the key normally
		strcpy((char*)*argKey, argv[2]);
	}

	// TODO: validate key length (64 bits for des, 128 bits for aes)

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
	int blockSize=8;
	ifstream inputFile;
	ofstream outputFile;

	argKey = (unsigned char*)malloc(sizeof(unsigned char) * 8);

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
		blockSize=16;
	}
	else if(argCipher == des)
	{
		blockSize=8;
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
	printf("SET KEY: %s\n", argKey);
	cipher->setKey(argKey);

	//open the input and output files
	inputFile.open((char*)argInputFile);
	outputFile.open((char*)argOutputFile);

	//make sure the input file is working
	if(!inputFile.is_open())
	{
		printf("Error opening file: %s\n", argInputFile);
		exit(-1);
	}

	//make sure the output file is working
	if(!outputFile.is_open())
	{
		printf("Error opening file: %s\n", argOutputFile);
		exit(-1);
	}

	// TODO: implement this pseudocode
	// ###############################

	// if DES
		// SIZE = 8
	// else
		// SIZE = 16

	// While there are still bytes in the input file, loop
	//
		// create a block datas tructure
		// pad block to SIZE bytes

		// create an output data structure
		// if encrypting
			// output = encrypt(stuff)
		// else
			// output = decrypt(stuff)

		// write output to file

	//perform encryption or decryption based on the argMode
	if(argMode == encrypt)
	{
		//encrypt specific variables
		char inputBlock[blockSize];
		char* paddedPlainText = NULL;
		int count = 0;
		unsigned char* cipherText= NULL;
		//read the data from the file
		//loop while the input file is reading 8 characters (64 bits) at a time
		printf("begging loop%d\n",blockSize);
		while(inputFile.read(inputBlock, blockSize))
		{
			printf("inputBlock %s\n", inputBlock);
			int len=sizeof(inputBlock)/sizeof(unsigned char);
			if( len<blockSize){
				pad_zero((unsigned char**)&inputBlock);	
			}
			count++;
			paddedPlainText = (char*)realloc(paddedPlainText, count * sizeof(char*) * blockSize);
			strcat(paddedPlainText, inputBlock);
			unsigned char* temp = cipher->encrypt((unsigned char*)inputBlock);
			printf("temp %s\n", temp);
			cipherText=(unsigned char*)realloc(cipherText, count * sizeof(unsigned char*) *blockSize);
			strcat((char*)cipherText,(char*) temp);
		}

		//with what is remaining in the inputBlock var after the loop terminates
		//pad it and push it onto the plain text
		//pad_zero((unsigned char**)&inputBlock);
		//count++;
		//paddedPlainText = (char*)realloc(paddedPlainText, count * sizeof(char*) * blockSize);
		//strcat(paddedPlainText, inputBlock);

		printf("PT: %s\n", paddedPlainText);
		printf("CT: %s\n", cipherText);
		/* Perform encryption */
		//unsigned char* cipherText = cipher->encrypt((unsigned char*)paddedPlainText);

		if(cipherText != NULL)
		{
			//write to output file
			int i = 0;
			while(cipherText[i] != '\0')
			{
				outputFile.put(cipherText[i]);
				i++;
			}
		}
		else
		{
			printf("ERROR: result of encryption is NULL\n");
		}

		//free the
		free(paddedPlainText);
		free(cipherText);
	}
	else if(argMode == decrypt)
	{
		//decrypt specific variables
		char inputBlock[8];
		char* cipherText = NULL;
		int count = 0;
		unsigned char* plainText=NULL;
		//read the data from the file
		//loop while the input file is reading 8 characters (64 bits) at a time
		while(inputFile.read(inputBlock, blockSize))
		{
			
			count++;
			unsigned char* temp=cipher->decrypt((unsigned char*)inputBlock);
			cipherText = (char*)realloc(cipherText, count * sizeof(char*) * blockSize);
			strcat(cipherText, inputBlock);
			plainText = (unsigned char*)realloc(plainText, count * sizeof(unsigned char*) * blockSize);
			strcat((char*)plainText,(char*) temp);
		}

		/* Perform decryption */
		//unsigned char* plainText = cipher->decrypt((unsigned char*)cipherText);
		printf("CT:%s\n",cipherText);
		printf("PT:%s\n", plainText);
		if(plainText != NULL)
		{
			//remove padding
			//pad_zero_remove((unsigned char**)&plainText);

			//write to output file
			int i = 0;
			while(plainText[i] != '\0')
			{
				outputFile.put(plainText[i]);
				i++;
			}
		}
		else
		{
			printf("ERROR: result of decryption is NULL\n");
		}

		//clean up memory
		free(cipherText);

		return 0;
	}
	else
	{
		printf("This should be un-reachable code\n");
		exit(-1);
	}

	free(argKey);
}
