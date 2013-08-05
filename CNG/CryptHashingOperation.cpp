#include "StdAfx.h"
#include "CryptHashingOperation.h"
#include "StringHelper.h"
#include <string>
#include <memory>

using namespace std;

CCryptHashingOperation::CCryptHashingOperation(void)
{
}


CCryptHashingOperation::~CCryptHashingOperation(void)
{
}

wstring CCryptHashingOperation::CalcHashInHexForm(wstring messageToHash, wstring hashAlgType, int digestLength)
{
	StringHelper stringHelper;
	vector<BYTE> resultHash;
	resultHash = CreateHash(messageToHash, hashAlgType);
	wstring resultHashToHex = stringHelper.ConvertByteArrayToString(&resultHash[0],digestLength);
	
	return resultHashToHex;
}

vector<BYTE> CCryptHashingOperation::CalcRawHash(wstring messageToHash, wstring hashAlgType, int digestLength)
{
	vector<BYTE> resultHash;
	resultHash = CreateHash(messageToHash, hashAlgType);
	return resultHash;
	
}

vector<BYTE> CCryptHashingOperation::CreateHash(wstring messageToHash, wstring hashAlgType)
{
	BCRYPT_ALG_HANDLE phAlgorithm;
	DWORD dwFlags = 0;

	BCRYPT_HASH_HANDLE hHash = NULL;

	PBYTE pbHashObject = NULL;
    PBYTE pbHash = NULL;

	DWORD cbHashObject = 0;
	DWORD cbData = 0;
	DWORD cbHash = 0;
	NTSTATUS res;
	
	vector<BYTE>  pbBufferCast = ConvertWStringToArrayOfByte(messageToHash);
	int lenBuffer = messageToHash.size();


	res = BCryptOpenAlgorithmProvider(&phAlgorithm, hashAlgType.c_str(), NULL, 0);
	
	 //calculate the size of the buffer to hold the hash object
    res = BCryptGetProperty(phAlgorithm, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0);

	////allocate the hash object on the heap
 //  
 //   
	pbHashObject = (PBYTE)HeapAlloc (GetProcessHeap (), 0, cbHashObject);
    if(NULL == pbHashObject)
    {
        wprintf(L"**** memory allocation failed\n");
    }

 //  //calculate the length of the hash
    res = BCryptGetProperty(phAlgorithm, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
 //   
 //   //allocate the hash buffer on the heap
    pbHash = (PBYTE)HeapAlloc (GetProcessHeap (), 0, cbHash);
    if(NULL == pbHash)
    {
        wprintf(L"**** memory allocation failed\n");
    }

	res = BCryptCreateHash(phAlgorithm, &hHash, pbHashObject,cbHashObject,NULL,0,0);
	//    
	res = BCryptHashData(hHash, &pbBufferCast[0],lenBuffer,0);
 //   
 //   //close the hash
	res = BCryptFinishHash(hHash, pbHash, cbHash, 0);

	//
    if(phAlgorithm)
    {
        BCryptCloseAlgorithmProvider(phAlgorithm,0);
    }

    if (hHash)    
    {
        BCryptDestroyHash(hHash);
    }

    if(pbHashObject)
    {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
    }

	vector<BYTE> dest;
	dest.resize(cbHash);
	memcpy(&dest[0], pbHash, cbHash*sizeof(BYTE));

	if(pbHash)
    {
        HeapFree(GetProcessHeap(), 0, pbHash);
		pbHash = nullptr;
    }
		
	return dest;

}

vector<BYTE> CCryptHashingOperation::ConvertWStringToArrayOfByte(wstring stringToConvert)
{
	size_t lenBuffer = stringToConvert.size();
	
	vector<BYTE> arrayOfByte;
	arrayOfByte.resize(lenBuffer);
			
	//convert text into BYTE 
	for(size_t i=0; i<lenBuffer;i++)
	{
		arrayOfByte[i] = (BYTE)stringToConvert.at(i);
	}
	return arrayOfByte;
}

