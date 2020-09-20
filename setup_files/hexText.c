################################################################################
# Author: Aviv Shabtay
# This program used to convert sentences in ASCII-string to Hex-String format.
################################################################################

#include <stdio.h>
#include <string.h>

void getHexStringFromAsciiString(char* result, char* data, int dataLen);
void getAsciiStringFromHexString(char* result, char* data, int resultLen);
int hexToInt(char c);
int hexToAscii(char c, char d);

int main()
{
    char text[] = "Joe Cocker - Unchain My Heart";
    int len = strlen(text);
    char textHex[len*2];
    char ascii[len];
    
    getHexStringFromAsciiString(textHex, text, len);
    getAsciiStringFromHexString(ascii, textHex, len);    
    
    printf("Hex is %s\n", textHex);
    printf("Ascii is %s\n", ascii);
    printf("Length is %d\n", len);
    
    char text2[] = "You snooze you lose";
    int len2 = strlen(text2);
    char textHex2[len2*2];
    char ascii2[len2];
    
    getHexStringFromAsciiString(textHex2, text2, len2);
    getAsciiStringFromHexString(ascii2, textHex2, len2);    
    
    printf("Hex is %s\n", textHex2);
    printf("Ascii is %s\n", ascii2);
    printf("Length is %d\n", len2);
    
    char text3[] = "Keep on trying";
    int len3 = strlen(text3);
    char textHex3[len3*2];
    char ascii3[len3];
    
    getHexStringFromAsciiString(textHex3, text3, len3);
    getAsciiStringFromHexString(ascii3, textHex3, len3);    
    
    printf("Hex is %s\n", textHex3);
    printf("Ascii is %s\n", ascii3);
    printf("Length is %d\n", len3);

    char text4[] = "Belive in yourself   ";
    int len4 = strlen(text4);
    char textHex4[len4*2];
    char ascii4[len4];
    
    getHexStringFromAsciiString(textHex4, text4, len4);
    getAsciiStringFromHexString(ascii4, textHex4, len4);    
    
    printf("Hex is %s\n", textHex4);
    printf("Ascii is %s\n", ascii4);
    printf("Length is %d\n", len4);

    return 0;
}

void getHexStringFromAsciiString(char* result, char* data, int dataLen){
    for(int i = 0; i < dataLen; ++i){
        sprintf(result+i*2, "%02X", data[i]);
    }    
}

void getAsciiStringFromHexString(char* result, char* hexData, int resultLen) {
    char buf = 0;
    for(int i = 0; i < resultLen*2; i++){
        if(i % 2 != 0){
            result[i/2] = hexToAscii(buf, hexData[i]);
        }else{
            buf = hexData[i];
        }
    }
}

int hexToInt(char c){
    int first = c / 16 - 3;
    int second = c % 16;
    int result = first*10 + second;
    if(result > 9) result--;
    return result;
}

int hexToAscii(char c, char d){
    int high = hexToInt(c) * 16;
    int low = hexToInt(d);
    return high+low;
}
