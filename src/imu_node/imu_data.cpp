// SOURCE: https://www.kernel.org/doc/Documentation/i2c/dev-interface */


//Bibliotheken
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}


//Globale Variablen Definitionen
using namespace std;
int addr = 0x69;
int file;
int adapter_nr = 1;
char filename [20];

int main() {


//Linux Datei Öffnen

file = open("/dev/i2c-1", O_RDWR);

cout << "File descriptor: " << file << '\n';


if (file < 0)
{
    exit(1);
}

//I2C Bus Addresse auswählen die wir ansprechen möchten

if (ioctl(file, I2C_SLAVE, addr) < 0) {
    exit(1);
}


//Zugriff auf die ICM Register 
// SOURCE: https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf*/


__u8 r_acc_x_h = 0x2D;
__u8 r_acc_x_l = 0x2E;

__u8 r_acc_y_h = 0x2F;
__u8 r_acc_y_l = 0x30;

__u8 r_acc_z_h = 0x31;
__u8 r_acc_z_l = 0x32;

__u8 pwr = 0x06;

__u8 register_bank = 0x7F;

__u8 acc_co = 0x14;



//Sleep Mode ausschalten
int power = i2c_smbus_read_byte_data(file, pwr);

i2c_smbus_write_byte_data(file, pwr, 0x01);

cout << "Power Status 0x" << hex << power << '\n';


i2c_smbus_write_byte_data(file, register_bank, 0x20);
int acc_config = i2c_smbus_read_byte_data(file, acc_co);
if (acc_config < 0) {
    cout << "Keine Daten" << '\n';

    /* ERROR HANDLING: i2c transaction failed */
  }
cout << "Accelerometer Konfiguration: 0x"  << hex << acc_config << '\n';

i2c_smbus_write_byte_data(file, acc_co, 0x00);


int acc_sensitivity_setting = (acc_config & 0x06) >> 1 ;
cout << "Accelerometer Konfiguration: "  << dec << acc_sensitivity_setting << '\n';

int acc_sensitivity = 0;

switch(acc_sensitivity_setting)

{ case 0:
  acc_sensitivity = 16384;
  break;

  case 1:
  acc_sensitivity = 8192;
  break;

  case 2:
  acc_sensitivity = 4096;
  break;

  case 3:
  acc_sensitivity = 2048;
  break;

}


i2c_smbus_write_byte_data(file, register_bank, 0x00);

while (1) {
//Accelerometer X-Achse High Bytes auslesen
int acc_x_h = i2c_smbus_read_byte_data(file, r_acc_x_h);
  if (acc_x_h < 0) {
    cout << "Keine Daten" << '\n';

    /* ERROR HANDLING: i2c transaction failed */
  }
// cout << "Accelerometer X-Achse High: " << dec << acc_x_h << '\n';


//Accelerometer X-Achse Low Bytes auslesen
int acc_x_l = i2c_smbus_read_byte_data(file, r_acc_x_l);
  if (acc_x_l < 0) {
    cout << "Keine Daten" << '\n';

    /* ERROR HANDLING: i2c transaction failed */
  } 
// cout << "Accelerometer X-Achse Low: " << dec << acc_x_l << '\n';


// SOURCE: https://industrialmonitordirect.com/de/blogs/knowledgebase/combining-high-byte-low-byte-into-word-binary-math-method?srsltid=AfmBOoqPGT1rgdbHERlU07CDVI11ToKN9l4_3Tkd5s8zL_GK-xMJ23Xx/
// Accelerometer X-Achsen Werte zusammenführen
float acc_x_out = acc_x_h * 256 + acc_x_l;

float acc_x = acc_x_out / acc_sensitivity;

cout << "Accelerometer X-Achse " << acc_x << '\n';


}
return 0;

}





