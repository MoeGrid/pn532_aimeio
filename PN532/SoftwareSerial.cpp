#include "SoftwareSerial.h"

//
// Constructor
//
SoftwareSerial::SoftwareSerial()
{
	_peek_ongoing = 0;
}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
	end();
}

//
// Public methods
//
void SoftwareSerial::setCOM(char* com)
{
	// Set port
	std::string str_com(com);
	_serial.setPort(str_com);
}

void SoftwareSerial::begin(long speed)
{
	_serial.setBaudrate(speed);
	_serial.open();
}

void SoftwareSerial::end()
{
	_serial.close();
}

// Read a byte from the buffer
int SoftwareSerial::read()
{
	_peek_ongoing = 0;
	uint8_t buffer[1];
	size_t bytes_read = 0;
	try {
		bytes_read = _serial.read(buffer, 1);
		if (bytes_read != 1) throw bytes_read;
	}
	catch (...) {
		return -1;
	}
	return buffer[0];
}

int SoftwareSerial::available()
{
	return _serial.available() + _peek_ongoing;
}

size_t SoftwareSerial::write(uint8_t byte)
{
	uint8_t tmp[] = { byte };
	return _serial.write(tmp, 1);
}

size_t SoftwareSerial::write(const uint8_t* buffer, size_t size)
{
	if (size <= 0) return size;
	return _serial.write(buffer, size);
}

void SoftwareSerial::flush()
{
	_serial.flush();
}
