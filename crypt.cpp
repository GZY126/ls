// crypt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>


//--------------------------------------------------------------------
// Copyright (C) Microsoft.  All rights reserved.
// Example of signing a hash and 
// verifying the hash signature.
#pragma comment(lib, "crypt32.lib")

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void MyHandleError(char *s);

void main(void)
{
	//-------------------------------------------------------------------
	// Declare and initialize variables.

	HCRYPTPROV hProv;
	BYTE *pbBuffer = (BYTE *)"The data that is to be hashed and signed.";
	DWORD dwBufferLen = strlen((char *)pbBuffer) + 1;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;
	HCRYPTKEY hPubKey;
	BYTE *pbKeyBlob;
	BYTE *pbSignature;
	DWORD dwSigLen;
	DWORD dwBlobLen;

	//-------------------------------------------------------------------
	// Acquire a cryptographic provider context handle.

	if (CryptAcquireContext(
		&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		0))
	{
		printf("CSP context acquired.\n");
	}
	else
	{
		MyHandleError("Error during CryptAcquireContext.");
	}
	//-------------------------------------------------------------------
	// Get the public at signature key. This is the public key
	// that will be used by the receiver of the hash to verify
	// the signature. In situations where the receiver could obtain the
	// sender's public key from a certificate, this step would not be
	// needed.

	//在项目属性->C/C++->语言中的符合模式项选择否即可
	if (CryptGenKey(
		hProv,
		AT_KEYEXCHANGE,
		(2048U << 16) | CRYPT_EXPORTABLE,
		&hKey))
	{
		printf("The signature key has been acquired. \n");
	}
	else
	{
		MyHandleError("Error during CryptGetUserKey for signkey.");
	}
	
	//-------------------------------------------------------------------
	// Export the public key. Here the public key is exported to a 
	// PUBLICKEYBOLB so that the receiver of the signed hash can
	// verify the signature. This BLOB could be written to a file and
	// sent to another user.

	if (CryptExportKey(
		hKey,
		NULL,
		PUBLICKEYBLOB,
		0,
		NULL,
		&dwBlobLen))
	{
		printf("Size of the BLOB for the public key determined. \n");
	}
	else
	{
		MyHandleError("Error computing BLOB length.");
	}
	//-------------------------------------------------------------------
	// Allocate memory for the pbKeyBlob.

	if (pbKeyBlob = (BYTE*)malloc(dwBlobLen))
	{
		printf("Memory has been allocated for the BLOB. \n");
	}
	else
	{
		MyHandleError("Out of memory. \n");
	}
	//-------------------------------------------------------------------
	// Do the actual exporting into the key BLOB.

	if (CryptExportKey(
		hKey,
		NULL,
		PUBLICKEYBLOB,
		0,
		pbKeyBlob,
		&dwBlobLen))
	{
		printf("Contents have been written to the BLOB. \n");
	}
	else
	{
		MyHandleError("Error during CryptExportKey.");
	}
	//-------------------------------------------------------------------
	// Create the hash object.

	if (CryptCreateHash(
		hProv,
		CALG_MD5,
		0,
		0,
		&hHash))
	{
		printf("Hash object created. \n");
	}
	else
	{
		MyHandleError("Error during CryptCreateHash.");
	}
	//-------------------------------------------------------------------
	// Compute the cryptographic hash of the buffer.

	if (CryptHashData(
		hHash,
		pbBuffer,
		dwBufferLen,
		0))
	{
		printf("The data buffer has been hashed.\n");
	}
	else
	{
		MyHandleError("Error during CryptHashData.");
	}
	//-------------------------------------------------------------------
	// Determine the size of the signature and allocate memory.

	dwSigLen = 0;
	if (CryptSignHash(
		hHash,
		AT_SIGNATURE,
		NULL,
		0,
		NULL,
		&dwSigLen))
	{
		printf("Signature length %d found.\n", dwSigLen);
	}
	else
	{
		MyHandleError("Error during CryptSignHash.");
	}
	//-------------------------------------------------------------------
	// Allocate memory for the signature buffer.

	if (pbSignature = (BYTE *)malloc(dwSigLen))
	{
		printf("Memory allocated for the signature.\n");
	}
	else
	{
		MyHandleError("Out of memory.");
	}
	//-------------------------------------------------------------------
	// Sign the hash object.

	if (CryptSignHash(
		hHash,
		AT_SIGNATURE,
		NULL,
		0,
		pbSignature,
		&dwSigLen))
	{
		printf("pbSignature is the hash signature.\n");
	}
	else
	{
		MyHandleError("Error during CryptSignHash.");
	}
	//-------------------------------------------------------------------
	// Destroy the hash object.

	if (hHash)
		CryptDestroyHash(hHash);

	printf("The hash object has been destroyed.\n");
	printf("The signing phase of this program is completed.\n\n");

	//-------------------------------------------------------------------
	// In the second phase, the hash signature is verified.
	// This would most often be done by a different user in a
	// separate program. The hash, signature, and the PUBLICKEYBLOB
	// would be read from a file, an email message, 
	// or some other source.

	// Here, the original pbBuffer, pbSignature, szDescription. 
	// pbKeyBlob, and their lengths are used.

	// The contents of the pbBuffer must be the same data 
	// that was originally signed.

	//-------------------------------------------------------------------
	// Get the public key of the user who created the digital signature 
	// and import it into the CSP by using CryptImportKey. This returns
	// a handle to the public key in hPubKey.

	if (CryptImportKey(
		hProv,
		pbKeyBlob,
		dwBlobLen,
		0,
		0,
		&hPubKey))
	{
		printf("The key has been imported.\n");
	}
	else
	{
		MyHandleError("Public key import failed.");
	}
	//-------------------------------------------------------------------
	// Create a new hash object.

	if (CryptCreateHash(
		hProv,
		CALG_MD5,
		0,
		0,
		&hHash))
	{
		printf("The hash object has been recreated. \n");
	}
	else
	{
		MyHandleError("Error during CryptCreateHash.");
	}
	//-------------------------------------------------------------------
	// Compute the cryptographic hash of the buffer.

	if (CryptHashData(
		hHash,
		pbBuffer,
		dwBufferLen,
		0))
	{
		printf("The new hash has been created.\n");
	}
	else
	{
		MyHandleError("Error during CryptHashData.");
	}
	//-------------------------------------------------------------------
	// Validate the digital signature.

	if (CryptVerifySignature(
		hHash,
		pbSignature,
		dwSigLen,
		hPubKey,
		NULL,
		0))
	{
		printf("The signature has been verified.\n");
	}
	else
	{
		printf("Signature not validated!\n");
	}
	//-------------------------------------------------------------------
	// Free memory to be used to store signature.

	if (pbSignature)
		free(pbSignature);

	//-------------------------------------------------------------------
	// Destroy the hash object.

	if (hHash)
		CryptDestroyHash(hHash);

	//-------------------------------------------------------------------
	// Release the provider handle.

	if (hProv)
		CryptReleaseContext(hProv, 0);
} //  End of main

//-------------------------------------------------------------------
//  This example uses the function MyHandleError, a simple error
//  handling function, to print an error message to the  
//  standard error (stderr) file and exit the program. 
//  For most applications, replace this function with one 
//  that does more extensive error reporting.

void MyHandleError(char *s)
{
	fprintf(stderr, "An error occurred in running the program. \n");
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error number %x.\n", GetLastError());
	fprintf(stderr, "Program terminating. \n");
	exit(1);
} // End of MyHandleError
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
