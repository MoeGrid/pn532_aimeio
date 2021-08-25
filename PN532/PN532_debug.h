#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG

#ifdef DEBUG
#define DMSG(str)			printf_s(str)
#define DMSG_STR(str)       printf_s(str)
#define DMSG_HEX(num)		printf_s("%02X", num)
#define DMSG_INT(num)		printf_s("%d", num)
#else
#define DMSG(str)
#define DMSG_STR(str)
#define DMSG_HEX(num)
#define DMSG_INT(num)
#endif

#endif
