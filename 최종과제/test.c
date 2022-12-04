
#define _CRT_SECURE_NO_WARNINGS

#include "openssl/sha.h"
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h> 


//  hash 16���� ��� ������ ��
//const unsigned char pKey[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
const unsigned char pKey[16];

// ���̰�
//const unsigned char pIn[] = { 0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf };

unsigned char pIn[128];

// ���̾�ȣȭ�Ȱ�
unsigned char pCipher[16] = { 0, };

// ��ȣȭ�ؼ������� ��
//unsigned char pPlain[16] = { 0, };


int ascii_to_hex(char c);
void aes_ebc_128_Encrypt();
void sha_1();
//void aes_ebc_128_De();

int main()
{
    FILE* fp;
    // �����η� �������༭ �����������
    fp = fopen("C:\\Users\\a.txt", "rb");
    fgetc(fp);

    // ���� �� ������ Ȯ��
    if (fp == NULL) {
        printf("Unable to open file %s \n", fp);
        return NULL;
    }

   /* unsigned char sum;
    unsigned char c1;
    unsigned char c2;*/
   
    // 16 �̻��ϼ��� �����ϱ� �� �̻��϶��� ����� SIZE ������ ���� �ݺ� ����
    // FSEEK �� ���̸� �˷���
    int size = fseek(fp, 0, SEEK_END);
    fseek(fp, 0, SEEK_SET);
    size /= 16;
    int i = 0;

    printf("���� ���� �� : %s\n\n", *fp);
    // �ƽ�Ű �ڵ带 ��簪����
    while (1) {
        unsigned char c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        pIn[i++] = c;
    }

    // sha1 �Լ��� �ؽð� ����(Ű��)
    sha_1();
    
    
    // ������ Ű ���� 20�̶� 16���� �������� aesŰ������ ��� ����
    for (int i =0; i <= size; i++) {
        aes_ebc_128_Encrypt();
    }
    
    /*aes_ebc_128_De();*/
    return 0;
}

void sha_1() {
    // Ű��
    unsigned char ibuf[] = "tmdwnalsrl";
    // �ؽ�ȭ�� Ű��
    unsigned char obuf[20];

    SHA1(ibuf, strlen(ibuf), obuf);

    printf("�ؽ�Ű�� : ");
    for (int i = 0; i < 20; i++) {
        printf(" %02x ", obuf[i]);
    }
    printf("\n");
    printf("\n");

    for (int i = 0; i < 15; i++) {
        // pkey �� , obuf 16�ڸ������� ��� ����
        strcpy(pKey ,obuf);
    }
    return;
}

//��ȣȭ����
void aes_ebc_128_Encrypt() {
    AES_KEY stKey;
    memset(&stKey, 0, sizeof(AES_KEY));
    if (AES_set_encrypt_key(pKey, 128, &stKey) < 0)
        return;
    AES_ecb_encrypt(pIn, pCipher, &stKey, AES_ENCRYPT);
    printf("��ȣȭ�� ���� : ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", pCipher[i]);
    }
    printf("\n");
    printf("\n");
    return;
}

// �ƽ�Ű ���� HEX ������ �������ִ� �ڵ�



 //��ȣȭ����
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