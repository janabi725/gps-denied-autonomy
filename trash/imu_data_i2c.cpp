#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

int file;
int adapter_nr = 1;
char filename[20];


int main() {
snprintf(filename, 19, "/dev/i2c-1", adapter_nr);
file = open(filename, O_RDWR);
if (file < 0) {
  exit(1);
}

  int addr = 0x69;

if (ioctl(file, I2C_SLAVE, addr) < 0) {
  exit(1);
}
}