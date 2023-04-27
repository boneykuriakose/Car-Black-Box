#ifndef EXT_EEPROM_H
#define EXT_EEPROM_H

#define SLAVE_READ_EXT			0xA1
#define SLAVE_WRITE_EXT		0xA0

void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);

#endif
