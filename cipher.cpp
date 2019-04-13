#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"

using namespace std;

enum cipher {aes = 0, des = 1};
enum mode {encrypt, decrypt};

bool parseArguments(int argc,
		char** argv,
		enum cipher *argCipher,
		unsigned char** argKey,
		enum mode *argMode,
		unsigned char** argInputFile,
		unsigned char** argOutputFile)
{
  // Check number of arguments
  if(argc != 6)
  {
    printf("Incorrect number of arguments\n");
    return false;
  }

  // Parse the cipher
  if(strcmp(argv[1], "AES") == 0)
  {
    *argCipher = aes;
  }
  else if(strcmp(argv[1], "DES") == 0)
  {
    *argCipher = des;
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





	// Parse the input file
	*argInputFile = (unsigned char*)argv[4];

	// Parse the output file
	*argOutputFile = (unsigned char*)argv[5];

	return true;

}

int main(int argc, char** argv)
{

  /* Create an instance of the DES cipher */
  CipherInterface* cipher = NULL;

  // Create variables for the cipher and mode
  enum cipher argCipher;

  unsigned char* argKey = new unsigned char;
  enum mode argMode;
  unsigned char* argInputFile;
  unsigned char* argOutputFile;
  ifstream inputFile;
  ofstream outputFile;
  unsigned char* block;
  unsigned char* text_buffer;

  // Parse arguments
	if(!parseArguments(argc, argv, &argCipher, &argKey, &argMode, &argInputFile, &argOutputFile))
	{
		printf("Unable to parse arguments\n");
		return 0;
	}

	/* TODO I'm getting weird errors when I put this in the parseArguments function. See if anyone
	you guys can try to figure out how to get this code working in that function*/


	// Parse the key
	//if the cipher is aes then we need to add a '0' or '1' onto the front of the key
	//to indicate encryption or decryption
  string key = argv[2];

  if(argCipher == des)
  {

    for(int i = 0; i < 16; i++)

    	argKey[i] = key[i];


  }
  else
  {
    if(argMode == encrypt)
    {
      argKey[0] = 0;
    }
    else
    {
      argKey[0] = 1;
    }

    for(int i = 0; i < 17; i++)
      argKey[i] = key[i-1];

  }

  //initialize the cipher
  if(argCipher == aes) { cipher = new AES(); }
  else if(argCipher == des) { cipher = new DES(); }

  /* Error checks */
  if(!cipher)
  {
    fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",
        __FILE__, __FUNCTION__, __LINE__);
    exit(-1);
  }

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

  // Acquires the text of the file and its length
  stringstream buffer;
  buffer << inputFile.rdbuf();
  string plainText(buffer.str());

  int file_length = plainText.length() - 1;

  printf("SET KEY: %s\n", argKey);
	cipher->setKey(argKey);

  // Set up block and text buffer for AES/DES encryption or decryption
  block = new unsigned char;
  text_buffer = new unsigned char[17];


  // AES encryption or decryption
  if(argCipher == aes)
  {

    if(argMode == encrypt)
    {

      for(int i = 0; i < file_length; i += 16)
      {
        memset(block, 0, 17);

        for(int j = 0; j < 16; j++)
          block[j] = plainText[i + j];

        memset(text_buffer, 0, 17);

        text_buffer = cipher->encrypt(block);

        for(int j = 0; j < 16; j++)
          outputFile << text_buffer[j];

        outputFile.flush();
      }

      printf("AES Encryption Successful!\n");

    }
    else if(argMode == decrypt)
    {

			// TODO THE PROBLEM LIES IN THIS BLOCK OF CODE
      for(int i = 0; i < file_length; i += 16)
      {
        memset(block, 0, 17);

        for(int j = 0; j < 16; j++)
          block[j] = plainText[i + j];

        memset(text_buffer, 0, 17);

        text_buffer = cipher->decrypt(block);

        for(int j = 0; j < 16; j++)
          outputFile << text_buffer[j];

        outputFile.flush();
      }

      printf("AES Decryption Successful!\n");
    }

  }
  //DES encryption or decryption
  else if(argCipher == des)
  {
    // Executes DES Encryption
    if(argMode == encrypt)
    {

      for(int i = 0; i < file_length; i += 8)
      {
          for(int j = 0; j < 8; j++)
            block[j] = plainText[i + j];

          text_buffer = cipher->decrypt(block);

          for(int j = 0; j < 8; j++)
            outputFile << text_buffer[j];
      }

      printf("DES Encryption Successful!\n");

    }
    else if(argMode == decrypt)
    {
      // Executes DES Decryption
      for(int i = 0; i < file_length; i += 8)
      {
          for(int j = 0; j < 8; j++)
            block[j] = plainText[i + j];

          text_buffer = cipher->encrypt(block);

          for(int j = 0; j < 8; j++)
            outputFile << text_buffer[j];
      }

      printf("DES Decryption Successful!\n");
    }
  }
  else
  {
    printf("This should be un-reachable code\n");
    exit(-1);
  }

  free(argKey);
  free(block);
  free(text_buffer);

  inputFile.close();
  outputFile.close();

  return 0;
}
