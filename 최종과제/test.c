
#define _CRT_SECURE_NO_WARNINGS

#include "openssl/sha.h"
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h> 


//  hash 16개로 끊어서 저장할 값
//const unsigned char pKey[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
const unsigned char pKey[16];

// 평문이고
//const unsigned char pIn[] = { 0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf };

unsigned char pIn[128];

// 평문이암호화된거
unsigned char pCipher[16] = { 0, };

// 복호화해서나오는 평문
//unsigned char pPlain[16] = { 0, };


int ascii_to_hex(char c);
void aes_ebc_128_Encrypt();
void sha_1();
//void aes_ebc_128_De();

int main()
{
    FILE* fp;
    // 절대경로로 지정해줘서 수정해줘야함
    fp = fopen("C:\\Users\\a.txt", "rb");
    fgetc(fp);

    // 파일 잘 갔는지 확인
    if (fp == NULL) {
        printf("Unable to open file %s \n", fp);
        return NULL;
    }

   /* unsigned char sum;
    unsigned char c1;
    unsigned char c2;*/
   
    // 16 이상일수도 있으니깐 그 이상일때를 대비해 SIZE 변수를 통해 반복 수행
    // FSEEK 가 길이를 알려줌
    int size = fseek(fp, 0, SEEK_END);
    fseek(fp, 0, SEEK_SET);
    size /= 16;
    int i = 0;

    printf("파일 원본 평문 : %s\n\n", *fp);
    // 아스키 코드를 헥사값으로
    while (1) {
        unsigned char c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        pIn[i++] = c;
    }

    // sha1 함수로 해시값 생성(키값)
    sha_1();
    
    
    // 생성된 키 값은 20이라서 16으로 만들어줘야 aes키값으로 사용 가능
    for (int i =0; i <= size; i++) {
        aes_ebc_128_Encrypt();
    }
    
    /*aes_ebc_128_De();*/
    return 0;
}

void sha_1() {
    // 키값
    unsigned char ibuf[] = "tmdwnalsrl";
    // 해쉬화된 키값
    unsigned char obuf[20];

    SHA1(ibuf, strlen(ibuf), obuf);

    printf("해쉬키값 : ");
    for (int i = 0; i < 20; i++) {
        printf(" %02x ", obuf[i]);
    }
    printf("\n");
    printf("\n");

    for (int i = 0; i < 15; i++) {
        // pkey 에 , obuf 16자리까지만 끊어서 저장
        strcpy(pKey ,obuf);
    }
    return;
}

//암호화과정
void aes_ebc_128_Encrypt() {
    AES_KEY stKey;
    memset(&stKey, 0, sizeof(AES_KEY));
    if (AES_set_encrypt_key(pKey, 128, &stKey) < 0)
        return;
    AES_ecb_encrypt(pIn, pCipher, &stKey, AES_ENCRYPT);
    printf("암호화된 문자 : ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", pCipher[i]);
    }
    printf("\n");
    printf("\n");
    return;
}

// 아스키 값을 HEX 값으로 변경해주는 코드



 //복호화과정
//void aes_ebc_128_De() {
//    AES_KEY stKey;
//    memset(&stKey, 0, sizeof(AES_KEY));
//    if (AES_set_decrypt_key(pKey, 128, &stKey) < 0)
//        return;
//    AES_ecb_encrypt(pCipher, pPlain, &stKey, AES_DECRYPT);
//
//    for (int i = 0; i < 16; i++) {
//        printf("%02x ", pPlain[i]);
//    }
//    printf("\n");
//    printf("\n");
//    return;
//}