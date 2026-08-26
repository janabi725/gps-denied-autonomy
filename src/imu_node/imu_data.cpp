// SOURCE: https://www.kernel.org/doc/Documentation/i2c/dev-interface */


//Bibliotheken
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}


// Namespace Definitionen
using namespace std;

//Globale Variablen Definitionen

int addr = 0x69;
int file;
int adapter_nr = 1;
char filename [20];

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


//Funktion Deklaration

int linux_i2c_file_open(string str);
int linux_i2c_bus_adress_access(int file, int address);
int icm_cancel_sleep_mode(int file, int power_adress);
int register_bank_switch(int file, int register_adress, int change_adress);
int acc_config(int file, int acc_adress_config, int config_value);
int acc_sensitivity_check (int acc_configuration);
int sensor_value_bytes(int file, int sensor_value_adress);
int16_t high_low_bytes_merge(int high_bytes, int low_bytes);
float adjusted_sensor_value (int16_t sensor_value, int sensitivity);
float g_to_m_2_conversion(string message, float sensor_value);

//MAIN

int main() {

string sensor_message_X = "Accelerometer X-Achse";
string sensor_message_Y = "Accelerometer Y-Achse";
string sensor_message_Z = "Accelerometer Z-Achse";

int datei = linux_i2c_file_open("/dev/i2c-1");

linux_i2c_bus_adress_access(datei, addr);
icm_cancel_sleep_mode(datei, pwr);

register_bank_switch(datei, register_bank , 0x20);

int acc_config_new = acc_config(datei, acc_co, 0x00);
int acc_sens = acc_sensitivity_check(acc_config_new);

register_bank_switch(datei, register_bank, 0x00);
while(1) {

//X-Achse
int acc_x_h = sensor_value_bytes(datei, r_acc_x_h);
int acc_x_l = sensor_value_bytes(datei, r_acc_x_l);
int16_t acc_x_out = high_low_bytes_merge(acc_x_h, acc_x_l);
float acc_x_g = adjusted_sensor_value(acc_x_out, acc_sens);
float acc_x = g_to_m_2_conversion(sensor_message_X, acc_x_g);

//Y-Achse
int acc_y_h = sensor_value_bytes(datei, r_acc_y_h);
int acc_y_l = sensor_value_bytes(datei, r_acc_y_l);
int16_t acc_y_out = high_low_bytes_merge(acc_y_h, acc_y_l);
float acc_y_g = adjusted_sensor_value(acc_y_out, acc_sens);
float acc_y = g_to_m_2_conversion(sensor_message_Y, acc_y_g);


//Z-Achse
int acc_z_h = sensor_value_bytes(datei, r_acc_z_h);
int acc_z_l = sensor_value_bytes(datei, r_acc_z_l);
int16_t acc_z_out = high_low_bytes_merge(acc_z_h, acc_z_l);
float acc_z_g = adjusted_sensor_value(acc_z_out, acc_sens);
float acc_z = g_to_m_2_conversion(sensor_message_Z, acc_z_g);


float acc_sqrt = sqrt(pow(acc_x, 2) + pow(acc_y, 2)  + pow(acc_z, 2) );
cout << "Accelerometer Wurzel: " << acc_sqrt << '\n';
}

return 0;

}

//Funktionsdefinitionen 


//Linux Datei Öffnen
int linux_i2c_file_open(string str) {
  
  file = open(str.c_str(), O_RDWR);

  cout << "File descriptor: " << file << '\n';

  if (file < 0)
  {
      exit(1);
  }
  return file;
  }



//I2C Bus Addresse auswählen die wir ansprechen möchten
int linux_i2c_bus_adress_access(int file, int address)
  {
  if (ioctl(file, I2C_SLAVE, address) < 0) {
      exit(1);
  }
  return 0;
  }


//Sleep Mode ausschalten
int icm_cancel_sleep_mode(int file, int power_adress) {
  int power = i2c_smbus_read_byte_data(file, power_adress);
  int power_status = i2c_smbus_write_byte_data(file, power_adress, 0x01);
  if (power_status < 0){
    cout << "Problem beim Power Modus" << '\n';
  }
  int power_new = i2c_smbus_read_byte_data(file, power_adress);
  cout << "Power Status 0x" << hex << power_new << '\n';
  return 0;
}

//Register Bank Wechsel
int register_bank_switch(int file, int register_adress, int change_adress){

int register_def = i2c_smbus_write_byte_data(file, register_adress, change_adress);
if (register_def < 0)
{
  cout << "Problem beim Registerwechsel" << '\n';

}
return 0;
}

//Accelerometer Konfiguration
int acc_config(int file, int acc_adress_config, int config_value){
int acc_config = i2c_smbus_read_byte_data(file, acc_adress_config);
if (acc_config < 0) {
    cout << "Keine Daten" << '\n';
    exit(1);
    /* ERROR HANDLING: i2c transaction failed */
  }
cout << "Accelerometer Konfiguration: 0x"  << hex << acc_config << '\n';

int acc_config_set = i2c_smbus_write_byte_data(file, acc_adress_config, config_value);
if (acc_config_set < 0){
  cout << "Konfiguration fehlgeschlagen" << '\n';
  exit(1);

}
int acc_config_new = i2c_smbus_read_byte_data(file, acc_adress_config);
if (acc_config_new < 0){
  cout << "Auslesen fehlgeschlagen" << '\n';
  exit(1);

  
}
return acc_config_new;
}

//Accelerometer Sensitivität rauslesen
int acc_sensitivity_check (int acc_configuration){
int acc_sensitivity_setting = (acc_configuration & 0x06) >> 1 ;

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

return acc_sensitivity;
}

//Sensor Werte Bytes auslesen
int sensor_value_bytes(int file, int sensor_value_adress){

int acc_value = i2c_smbus_read_byte_data(file, sensor_value_adress);
  if (acc_value< 0) {
    cout << "Keine Daten" << '\n';
    exit(1);

    /* ERROR HANDLING: i2c transaction failed */
  }

return acc_value;
}

int16_t high_low_bytes_merge(int high_bytes, int low_bytes){
// SOURCE: https://industrialmonitordirect.com/de/blogs/knowledgebase/combining-high-byte-low-byte-into-word-binary-math-method?srsltid=AfmBOoqPGT1rgdbHERlU07CDVI11ToKN9l4_3Tkd5s8zL_GK-xMJ23Xx/
// Accelerometer X-Achsen Werte zusammenführen
int16_t value = high_bytes* 256 + low_bytes;

return value;

}


float adjusted_sensor_value (int16_t sensor_value, int sensitivity){
float clean_sensor_value = static_cast<float> (sensor_value) / static_cast<float> (sensitivity);
return clean_sensor_value;
}


float g_to_m_2_conversion(string message, float sensor_value){

float final_value = sensor_value * 9.81;

cout << message << ": " << final_value << '\n';

return final_value;
}