// test_pn532.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <windows.h>
#include <iostream>
#include "PN532_SWHSU.h"
#include "PN532.h"

SoftwareSerial hsu;
PN532_SWHSU pn532hsu = PN532_SWHSU(hsu);
PN532 nfc = PN532(pn532hsu);

uint8_t AimeKey[6] = { 0x57, 0x43, 0x43, 0x46, 0x76, 0x32 };
uint8_t BanaKey[6] = { 0x60, 0x90, 0xD0, 0x06, 0x32, 0xF5 };

int main()
{
	hsu.setCOM("COM10");

	nfc.begin();

	uint32_t version = nfc.getFirmwareVersion();
	if (version == 0)
	{
		printf_s("连接失败!\n");
		return 0;
	}
	printf_s("Found chip PN5%02X\nFirmware ver: %d.%d\n", (version >> 24) & 0xFF, (version >> 16) & 0xFF, (version >> 8) & 0xFF);

	nfc.setPassiveActivationRetries(0x10);
	nfc.SAMConfig();

	while (true)
	{
		uint8_t success; 
		uint16_t SystemCode;
		uint8_t IDm[8];
		uint8_t PMm[8];


		success = nfc.felica_Polling(0xFFFF, 0x00, IDm, PMm, &SystemCode, 0x0F);
		if (success) {
			printf("Found an Felica card");
			printf("  IDm Value: ");
			nfc.PrintHex(IDm, 8);
			printf("\n");
			printf("  PMm Value: ");
			nfc.PrintHex(PMm, 8);
			printf("\n");
		}
		/*
		uint8_t success;
		uint8_t uid[7] = { 0 };
		uint8_t uidLength;

		success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
		if (success) {
			printf("Found an ISO14443A card");
			printf("  UID Length: %d bytes", uidLength);
			printf("  UID Value: ");
			nfc.PrintHex(uid, uidLength);
			printf("\n");

			if (uidLength == 4)
			{
				success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 1, 1, AimeKey);
				if (success && uidLength == 4)
				{
					uint8_t data[16];
					success = nfc.mifareclassic_ReadDataBlock(2, data);
					Sleep(100);
					if (success)
					{
						printf("Reading Block 4: ");
						nfc.PrintHex(data, 16);
						printf("\n");

						Sleep(1000);
					}
					else
					{
						printf("Ooops ... unable to read the requested block.  Try another key?\n");
					}
				}
				else
				{
					printf("Ooops ... authentication failed: Try another key?\n");
				}
			}
		}
		*/
	}
}
