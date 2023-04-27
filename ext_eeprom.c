#include "ext_eeprom.h"
#include <xc.h>
#include "i2c.h"

void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_EXT);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
	for(int wait = 50000;wait--;);
}

unsigned char read_ext_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_EXT);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_EXT);
	data = i2c_read();
	i2c_stop();

	return data;
}
