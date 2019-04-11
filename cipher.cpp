#include <string>
#include <fstream>
#include <vector>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

//enums for the cipher and mode arguments
enum cipher {aes, des};
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

	ifstream inputFile;
	ofstream outputFile;

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

	//perform encryption or decryption based on the argMode
	if(argMode == encrypt)
	{
		//encrypt specific variables
		char inputBlock[8];
		char* paddedPlainText = NULL;
		int count = 0;

		//read the data from the file
		//loop while the input file is reading 8 characters (64 bits) at a time
		while(inputFile.read(inputBlock, 8))
		{
			count++;
			paddedPlainText = (char*)realloc(paddedPlainText, count * sizeof(char*) * 8);
			strcat(paddedPlainText, inputBlock);
		}

		//with what is remaining in the inputBlock var after the loop terminates
		//pad it and push it onto the plain text
		pad_zero((unsigned char**)&inputBlock);
		count++;
		paddedPlainText = (char*)realloc(paddedPlainText, count * sizeof(char*) * 8);
		strcat(paddedPlainText, inputBlock);

		printf("PT: %s\n", paddedPlainText);
		
		/* Perform encryption */
		unsigned char* cipherText = cipher->encrypt((unsigned char*)paddedPlainText);

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
	}
	else if(argMode == decrypt)
	{
		//decrypt specific variables
		char inputBlock[8];
		char* cipherText = NULL;
		int count = 0;

		//read the data from the file
		//loop while the input file is reading 8 characters (64 bits) at a time
		while(inputFile.read(inputBlock, 8))
		{
			count++;
			cipherText = (char*)realloc(cipherText, count * sizeof(char*) * 8);
			strcat(cipherText, inputBlock);
		}

		/* Perform decryption */
		unsigned char* plainText = cipher->decrypt((unsigned char*)cipherText);	

		if(plainText != NULL)
		{
			//remove padding
			pad_zero_remove((unsigned char**)&plainText);

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
}
