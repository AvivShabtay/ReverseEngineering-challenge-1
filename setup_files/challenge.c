################################################################################
# Author: Aviv Shabtay
# This program is the challenge itself.
################################################################################

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#pragma warning (disable: 4996)

#define BUFSIZE 1024
#define MD5LEN  16
#define MAX_PASSWORD 100
#define MAX_STRING 100

void getHexStringFromAsciiString(char* result, char* data, int dataLen);
void getAsciiStringFromHexString(char* result, char* data, int resultLen);
int hexToInt(char c);
int hexToAscii(char c, char d);

DWORD main(int argc, CHAR* argv[])
{
	DWORD dwStatus = 0;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789ABCDEF";
	CHAR password[MAX_PASSWORD];
	CHAR hashedPassword[MD5LEN * 2 + 1];
	DWORD matchNumber = -1;

	// Hashes:
	CHAR hash1[] = "C53E479B03B3220D3D56DA88C4CACE20";
	CHAR hash2[] = "3289815147D05C60A2FCF85B389D7FCE";
	CHAR hash3[] = "A01726B559EEEB5FC287BF0098A22F6C";
	CHAR hash4[] = "ECE4947EDA94C6591B2F9D59CADCAB6A";

	CHAR result1[] = "4A6F6520436F636B6572202D20556E636861696E204D79204865617274";
	CHAR result2[] = "596F7520736E6F6F7A6520796F75206C6F7365";
	CHAR result3[] = "4B656570206F6E20747279696E67";
	CHAR result4[] = "42656C69766520696E20796F757273656C66202020";

	// Get password from the user:
	if (argc < 2) {
		printf("Usage: challenge.exe <your_password>\n");
		return 1;
	}
	printf("Welcome!!!\n");
	//printf("[+] Please enter password to get the flag: ");
	strcpy(password, argv[1]);
	//dwStatus = scanf("%s[^\n]%*c", password);
	printf("[+] Your password is: %s\n", password);

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		dwStatus = GetLastError();
		printf("[-] CryptAcquireContext failed: %d\n", dwStatus);
		return dwStatus;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		dwStatus = GetLastError();
		printf("[-] CryptAcquireContext failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		return dwStatus;
	}

	// Get the password length:
	cbRead = strlen(password);

	// Updates the Hash object with some data:
	if (!CryptHashData(hHash, password, cbRead, 0)) {
		dwStatus = GetLastError();
		printf("[-] CryptHashData failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		return dwStatus;
	}

	cbHash = MD5LEN;

	// Get the actual hash value:
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
		printf("[+] MD5 hash of given password %s is: ", password);
		for (DWORD i = 0; i < cbHash; i++) {
			//sprintf("%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
			int j = i * 2;
			hashedPassword[j] = rgbDigits[rgbHash[i] >> 4];
			hashedPassword[j + 1] = rgbDigits[rgbHash[i] & 0xf];
		}
		hashedPassword[MD5LEN * 2] = '\0';
		printf("%s", hashedPassword);
		printf("\n");
	}
	else {
		dwStatus = GetLastError();
		printf("[-] CryptGetHashParam failed: %d\n", dwStatus);
	}

	// Check if the user-hashed-password match any saved hashes:
	if (strcmp(hashedPassword, hash1) == 0) {
		matchNumber = 1;
	}
	else if (strcmp(hashedPassword, hash2) == 0) {
		matchNumber = 2;
	}
	else if (strcmp(hashedPassword, hash3) == 0) {
		matchNumber = 3;
	}
	else if (strcmp(hashedPassword, hash4) == 0) {
		matchNumber = 4;
	}

	switch (matchNumber) {
	case 1:
	{
		int len = strlen(result1) / 2;
		char ascii[MAX_STRING];
		getAsciiStringFromHexString(ascii, result1, len);
		printf("[+] Nice!\n");
		printf("[+] %s\n", ascii);
		break;
	}
	case 2:
	{
		int len = strlen(result2) / 2;
		char ascii[MAX_STRING];
		getAsciiStringFromHexString(ascii, result2, len);
		printf("[-] Wrong!\n");
		printf("[+] %s\n", ascii);
		break;
	}
	case 3:
	{
		int len = strlen(result3) / 2;
		char ascii[MAX_STRING];
		getAsciiStringFromHexString(ascii, result3, len);
		printf("[-] Wrong!\n");
		printf("[+] %s\n", ascii);
		break;
	}
	case 4:
	{
		int len = strlen(result4) / 2;
		char ascii[MAX_STRING];
		getAsciiStringFromHexString(ascii, result4, len);
		printf("[-] Wrong!\n");
		printf("[+] %s\n", ascii);
		break;
	}
	default:
	{
		printf("[-] The password didn't match any saved password\n");
		break;
	}
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	printf("[+] Goodbye!\n");

	return dwStatus;
}

void getHexStringFromAsciiString(char* result, char* data, int dataLen) {
	for (int i = 0; i < dataLen; ++i) {
		sprintf(result + i * 2, "%02X", data[i]);
	}
}

void getAsciiStringFromHexString(char* result, char* hexData, int resultLen) {
	char buf = 0;
	for (int i = 0; i < resultLen * 2; i++) {
		if (i % 2 != 0) {
			result[i / 2] = hexToAscii(buf, hexData[i]);
		}
		else {
			buf = hexData[i];
		}
	}
}

int hexToInt(char c) {
	int first = c / 16 - 3;
	int second = c % 16;
	int result = first * 10 + second;
	if (result > 9) result--;
	return result;
}

int hexToAscii(char c, char d) {
	int high = hexToInt(c) * 16;
	int low = hexToInt(d);
	return high + low;
}

