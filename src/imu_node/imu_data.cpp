// SOURCE: https://www.kernel.org/doc/Documentation/i2c/dev-interface */
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c-smbus.h>



using namespace std;
int addr = 0x69;
int file;
int adapter_nr = 1;
char filename [20];

int main() {

file = open("/dev/i2c-1", O_RDWR);
if (file < 0)
{
    exit(1);
}

if (ioctl(file, I2C_SLAVE, addr) < 0) {
    exit(1);
}

__u8 reg = 0x00;
__s32 res;
char buf [10];

int response = i2c_smbus_read_word_data(file, reg);
  if (res < 0) {
    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* res contains the read word */
  }

 if (read(file, buf, 1) != 1) {
    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* buf[0] contains the read byte */
  }

  cout << response;

file.close();

return 0;

}





