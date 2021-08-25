#ifndef __SOFTWARE_SERIAL_H__
#define __SOFTWARE_SERIAL_H__

#include "serial/serial.h"

class SoftwareSerial
{
private:
	char _COM_port[128];
	int _peek_char;
	bool _peek_ongoing;
	serial::Serial _serial;
public:
	void setCOM(char* com);

	SoftwareSerial();
	~SoftwareSerial();
	void begin(long speed);
	void end();

	size_t write(uint8_t byte);
	size_t write(const uint8_t* b, size_t size);
	int read();
	int available();
	void flush();
};

#endif
