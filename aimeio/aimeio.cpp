#include <windows.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aimeio.h"

#include "crc.h"
#include "dprintf.h"

#include "PN532_SWHSU.h"
#include "PN532.h"

SoftwareSerial hsu;
PN532_SWHSU pn532i2c(hsu);
PN532 nfc(pn532i2c);

uint8_t AimeKey[6] = { 0x57, 0x43, 0x43, 0x46, 0x76, 0x32 };
uint8_t BanaKey[6] = { 0x60, 0x90, 0xD0, 0x06, 0x32, 0xF5 };

uint8_t card_type, mifare_uid[4], felica_IDm[8], felica_PMm[8];
uint16_t systemCode = 0xFFFF;
uint8_t requestCode = 0x01;
uint16_t systemCodeResponse;

struct aime_io_config {
	wchar_t com[MAX_PATH];
};

static struct aime_io_config aime_io_cfg;
static uint8_t aime_io_felica_id[8];
static HANDLE hSection;
static int* secData;
int* ffbOffset;
int* ffbOffset2;
int* ffbOffset3;
int* ffbOffset4;

/// <summary>
/// 读取配置
/// </summary>
/// <param name="cfg"></param>
/// <param name="filename"></param>
static void aime_io_config_read(
	struct aime_io_config* cfg,
	const wchar_t* filename)
{
	assert(cfg != NULL);
	assert(filename != NULL);

	GetPrivateProfileStringW(
		L"aimeio",
		L"com",
		L"COM1",
		cfg->com,
		_countof(cfg->com),
		filename);
}

/// <summary>
/// 初始化
/// </summary>
/// <param name=""></param>
/// <returns></returns>
HRESULT aime_io_init(void)
{
	aime_io_config_read(&aime_io_cfg, L".\\segatools.ini");

	// 初始化串口
	char tmp[MAX_PATH] = { 0 };
	sprintf_s(tmp, sizeof(tmp), "%ws", aime_io_cfg.com);
	hsu.setCOM(tmp);

	// 初始化PN532
	nfc.begin();
	if (!nfc.getFirmwareVersion())
	{
		dprintf("PN532 connection failed!\n");
		return S_FALSE;
	}
	
	nfc.setPassiveActivationRetries(0x10); // 设定等待次数
	nfc.SAMConfig();

	return S_OK;
}

void aime_io_fini(void)
{

}

/// <summary>
/// 轮询
/// </summary>
/// <param name="unit_no"></param>
/// <returns></returns>
HRESULT aime_io_nfc_poll(uint8_t unit_no)
{
	if (unit_no != 0)
		return S_OK;

	// 开始读卡
	memset(&mifare_uid, 0, 4);
	memset(&felica_IDm, 0, 8);
	memset(&felica_PMm, 0, 8);

	uint8_t u_len;
	if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, mifare_uid, &u_len) && nfc.mifareclassic_AuthenticateBlock(mifare_uid, u_len, 1, 1, AimeKey))
		card_type = 1;
	else if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, mifare_uid, &u_len) && nfc.mifareclassic_AuthenticateBlock(mifare_uid, u_len, 1, 0, BanaKey))
		card_type = 1;
	else if (nfc.felica_Polling(systemCode, requestCode, felica_IDm, felica_PMm, &systemCodeResponse, 200))
		card_type = 2;
	else
		card_type = 0;

	return S_OK;
}

/// <summary>
/// 获取Aime ID
/// </summary>
/// <param name="unit_no"></param>
/// <param name="luid"></param>
/// <param name="luid_size"></param>
/// <returns></returns>
HRESULT aime_io_nfc_get_aime_id(
	uint8_t unit_no,
	uint8_t* luid,
	size_t luid_size)
{
	assert(luid != NULL);

	if (unit_no != 0 || card_type != 1)
		return S_FALSE;

	uint8_t data[16];
	if (!nfc.mifareclassic_ReadDataBlock(2, data))
	{
		dprintf("Failed to read card!\n");
		return S_FALSE;
	}
	memcpy(luid, data + 6, luid_size);

	return S_OK;
}

/// <summary>
/// 获取felica ID
/// </summary>
/// <param name="unit_no"></param>
/// <param name="IDm"></param>
/// <returns></returns>
HRESULT aime_io_nfc_get_felica_id(uint8_t unit_no, uint64_t* IDm)
{
	assert(IDm != NULL);

	if (unit_no != 0 || card_type != 2) {
		return S_FALSE;
	}
	memcpy(IDm, felica_IDm, 8);

	return S_OK;
}

/// <summary>
/// LED设置颜色
/// </summary>
/// <param name="unit_no"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
void aime_io_led_set_color(uint8_t unit_no, uint8_t r, uint8_t g, uint8_t b)
{

}