// SOURCE: https://www.kernel.org/doc/Documentation/i2c/dev-interface */
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}



using namespace std;
int addr = 0x69;
int file;
int adapter_nr = 1;
char filename [20];

int main() {

file = open("/dev/i2c-1", O_RDWR);

cout << "File descriptor: " << file << '\n';


if (file < 0)
{
    exit(1);
}

if (ioctl(file, I2C_SLAVE, addr) < 0) {
    exit(1);
}

// SOURCE: https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf*/
__u8 reg = 0x00;

__u8 r_acc_x_h = 0x2D;
__u8 r_acc_x_l = 0x2E;

__u8 r_acc_y_h = 0x2F;
__u8 r_acc_y_l = 0x30;

__u8 r_acc_z_h = 0x31;
__u8 r_acc_z_l = 0x32;

__u8 pwr = 0x06;



int response = i2c_smbus_read_byte_data(file, reg);
  if (response < 0) {
    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* res contains the read word */
  }
cout << "Response: 0x" << hex << response << '\n';


int power = i2c_smbus_read_byte_data(file, pwr);

i2c_smbus_write_byte_data(file, pwr, 0x01);

cout << "Power Status 0x" << hex << power << '\n';



int acc_x_h = i2c_smbus_read_byte_data(file, r_acc_x_h);
  if (acc_x_h < 0) {
    cout << "Keine Daten" << '\n';

    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* res contains the read word */
  }
cout << "Accelerometer X-Achse High: " << dec << acc_x_h << '\n';

int acc_x_l = i2c_smbus_read_byte_data(file, r_acc_x_l);
  if (acc_x_l < 0) {
    cout << "Keine Daten" << '\n';

    /* ERROR HANDLING: i2c transaction failed */
  } else {
    /* res contains the read word */
  }
cout << "Accelerometer X-Achse Low: " << dec << acc_x_l << '\n';



return 0;

}





