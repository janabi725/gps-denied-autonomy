// SOURCE: https://www.kernel.org/doc/Documentation/i2c/dev-interface */


//Bibliotheken
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>
#include <string>
#include <tuple>
#include <iomanip>
#include "imu_driver_ros/icm.hpp"


extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}


// Namespace Definitionen
using namespace std;

//Globale Variablen Definitionen

// int addr = 0x69;
int adapter_nr = 1;
const double pi = 3.14159265358979323846;

//Zugriff auf die ICM Register 
// SOURCE: https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000189-icm-20948-v1.5.pdf*/

__u8 r_acc_x_h = 0x2D;
__u8 r_acc_x_l = 0x2E;

__u8 r_acc_y_h = 0x2F;
__u8 r_acc_y_l = 0x30;

__u8 r_acc_z_h = 0x31;
__u8 r_acc_z_l = 0x32;


__u8 r_gyro_x_h = 0x33; 
__u8 r_gyro_x_l = 0x34;
__u8 r_gyro_y_h = 0x35;
__u8 r_gyro_y_l = 0x36;
__u8 r_gyro_z_h = 0x37;
__u8 r_gyro_z_l = 0x38;

__u8 r_temp_h = 0x39;
__u8 r_temp_l = 0x3A;

__u8 r_mag_x_l = 0x3B;
__u8 r_mag_x_h = 0x3C;
__u8 r_mag_y_l = 0x3D;
__u8 r_mag_y_h = 0x3E;
__u8 r_mag_z_l = 0x3F;
__u8 r_mag_z_h = 0x40;
__u8 r_tmps = 0x41;
__u8 r_st2 = 0x42;


//__u8 pwr = 0x06;

__u8 register_bank = 0x7F;

__u8 acc_co = 0x14;
__u8 gyro_co = 0x01;


__u8 user_control = 0x03;
__u8 i2c_master_control = 0x01;
__u8 i2c_slave0_adress = 0x03;
__u8 i2c_slave0_register = 0x04;
__u8 i2c_slave0_control = 0x05;
__u8 i2c_slave0_DO = 0x06;




//Funktion Deklaration

int register_bank_switch(int file, int register_adress, int change_adress);
int acc_sensitivity_check (int acc_configuration);
float gyro_sensitivity_check(int gyro_configuration);
int16_t high_low_bytes_merge(int high_bytes, int low_bytes);
float adjusted_sensor_value (int16_t sensor_value, float sensitivity);
float g_to_m_2_conversion(float sensor_value);
float degrees_to_radian_conversion(float sensor_value);
float temperature_measure (float sensor_value);
int read_register(int file, int register_adress);
void write_register(int file, int register_adress, int value);
float magnetometer_sensitivity_mikro(int16_t value);
double magnetometer_sensitivity_tesla(int value);



//Funktionsdefinitionen 


//Linux Datei Öffnen
int linux_i2c_file_open(string str) {

  int datei;
  
  datei = open(str.c_str(), O_RDWR);

  cout << "File descriptor: " << datei << '\n';

  if (datei < 0)
  {
      exit(1);
  }
  return datei;
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
  int power = read_register(file, power_adress);
  write_register(file, power_adress, 0x01);
  return 0;
}

//Register Bank Wechsel
int register_bank_switch(int file, int register_adress, int change_adress){

write_register(file, register_adress, change_adress);

return 0;
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





//Gyroskop Sensitivität
float gyro_sensitivity_check(int gyro_configuration){

int gyro_sensitivity_setting = (gyro_configuration & 0x06) >> 1 ;

cout << "Gyroskop Konfiguration: "  << dec << gyro_sensitivity_setting << '\n';

float gyro_sensitivity = 0;

switch(gyro_sensitivity_setting)

{ case 0:
  gyro_sensitivity = 131;
  break;

  case 1:
  gyro_sensitivity = 65.5;
  break;

  case 2:
  gyro_sensitivity = 32.8;
  break;

  case 3:
  gyro_sensitivity = 16.4;
  break;

}

return gyro_sensitivity;
}


//High und Low Bytes Zusammenführen
int16_t high_low_bytes_merge(int high_bytes, int low_bytes){
// SOURCE: https://industrialmonitordirect.com/de/blogs/knowledgebase/combining-high-byte-low-byte-into-word-binary-math-method?srsltid=AfmBOoqPGT1rgdbHERlU07CDVI11ToKN9l4_3Tkd5s8zL_GK-xMJ23Xx/
// Accelerometer X-Achsen Werte zusammenführen
int16_t value = high_bytes* 256 + low_bytes;

return value;

}


//Sensitivität für ACC und GYRO anpassen
float adjusted_sensor_value (int16_t sensor_value, float sensitivity){
float clean_sensor_value = static_cast<float> (sensor_value) / static_cast<float> (sensitivity);
return clean_sensor_value;
}



//Erdgravitation anpassen
float g_to_m_2_conversion(float sensor_value){

float final_value = sensor_value * 9.81;


return final_value;
}


//Winkel zu Grad pro Sekunde
float degrees_to_radian_conversion(float sensor_value)
{
float final_value = sensor_value * (pi / 180);

return final_value;

}


//Temperatur Messung

float temperature_measure (float sensor_value){

float final_value = ((sensor_value - 0)/ 333.87)+ 21;


return final_value;
}




//Register Lesen
int read_register(int file, int register_adress){
int value = i2c_smbus_read_byte_data(file,register_adress);
  if (value< 0) {
    cout << "Keine Daten zum Auslesen" << '\n';
    exit(1);

    /* ERROR HANDLING: i2c transaction failed */
  }

return value;
}


//Register Schreiben
void write_register(int file, int register_adress, int value){
int status = i2c_smbus_write_byte_data(file, register_adress, value);
if (status < 0){
  cout << "Schreiben fehlgeschlagen" << '\n';
  exit(1);

  
}

}


//Magnetometer Sensitivität für Mikrotesla

float magnetometer_sensitivity_mikro(int16_t value){

  float magnetic_field_uT = static_cast<float> (value) * 0.15;
  return magnetic_field_uT; 

}


//Magenetometer Sensitivität für Tesla

double magnetometer_sensitivity_tesla(int value){

  double magnetic_field_T =static_cast<double> (value) * 0.15 * 1e-6;
  return magnetic_field_T; 

}

//Accelerometer Konfiguration

int configure_accelerometer(int file){

register_bank_switch(file, register_bank , 0x20);
write_register(file, acc_co, 0x00);
int acc_config_new = read_register(file, acc_co);
int acc_sens = acc_sensitivity_check(acc_config_new);
register_bank_switch(file, register_bank, 0x00);

return acc_sens ;}


//Gyroskop Konfiguration
float  configure_gyroskop(int file){

register_bank_switch(file, register_bank , 0x20);
write_register(file, gyro_co, 0x00);
int gyro_config_new = read_register(file, gyro_co);
float gyro_sens = gyro_sensitivity_check(gyro_config_new);
register_bank_switch(file, register_bank, 0x00);

return gyro_sens;
}




//Magnetometer Konfiguration
int configure_magnetometer(int file){
register_bank_switch(file, register_bank, 0x00);
int user_control_value = read_register(file, user_control);
user_control_value = user_control_value | 0x20;
write_register(file, user_control, user_control_value);
int test = read_register(file,user_control);
cout << "User Control Register: " << hex << test << '\n';

register_bank_switch(file, register_bank, 0x30);
write_register(file, i2c_master_control, 0x07);
write_register(file, i2c_slave0_adress, 0x0C);
write_register(file, i2c_slave0_register, 0x31);
write_register(file, i2c_slave0_DO, 0x08);
write_register(file, i2c_slave0_control, 0x81);
usleep(500);
write_register(file, i2c_slave0_adress, 0x8C);
write_register(file, i2c_slave0_register, 0x11);
write_register(file, i2c_slave0_control, 0x88);
register_bank_switch(file, register_bank, 0x00);

return 0;}


//Accelerometer Auslesen
struct Vector read_accelerometer(int file, int acc_sens){

Vector Acc;

//X-Achse
int acc_x_h = read_register(file, r_acc_x_h);
int acc_x_l = read_register(file, r_acc_x_l);
int16_t acc_x_out = high_low_bytes_merge(acc_x_h, acc_x_l);
float acc_x_g = adjusted_sensor_value(acc_x_out, acc_sens);
float acc_x = g_to_m_2_conversion(acc_x_g);

//Y-Achse
int acc_y_h = read_register(file, r_acc_y_h);
int acc_y_l = read_register(file, r_acc_y_l);
int16_t acc_y_out = high_low_bytes_merge(acc_y_h, acc_y_l);
float acc_y_g = adjusted_sensor_value(acc_y_out, acc_sens);
float acc_y = g_to_m_2_conversion(acc_y_g);

//Z-Achse
int acc_z_h = read_register(file, r_acc_z_h);
int acc_z_l = read_register(file, r_acc_z_l);
int16_t acc_z_out = high_low_bytes_merge(acc_z_h, acc_z_l);
float acc_z_g = adjusted_sensor_value(acc_z_out, acc_sens);
float acc_z = g_to_m_2_conversion(acc_z_g);


Acc.x = acc_x;
Acc.y = acc_y;
Acc.z = acc_z;

return (Acc);


}



//Gyroskop Auslesen

struct Vector read_gyroksop(int file, float gyro_sens){
Vector Gyro;

//X-Achse
int gyro_x_h = read_register(file, r_gyro_x_h);
int gyro_x_l = read_register(file, r_gyro_x_l);
int16_t gyro_x_out = high_low_bytes_merge(gyro_x_h, gyro_x_l);
float gyro_x_dps = adjusted_sensor_value(gyro_x_out, gyro_sens);
float gyro_x = degrees_to_radian_conversion(gyro_x_dps);

//Y-Achse
int gyro_y_h = read_register(file, r_gyro_y_h);
int gyro_y_l = read_register(file, r_gyro_y_l);
int16_t gyro_y_out = high_low_bytes_merge (gyro_y_h, gyro_y_l);
float gyro_y_dps = adjusted_sensor_value (gyro_y_out, gyro_sens);
float gyro_y = degrees_to_radian_conversion(gyro_y_dps);

//Z-Achse
int gyro_z_h = read_register(file, r_gyro_z_h);
int gyro_z_l = read_register(file, r_gyro_z_l);
int16_t gyro_z_out = high_low_bytes_merge (gyro_z_h, gyro_z_l);
float gyro_z_dps = adjusted_sensor_value (gyro_z_out, gyro_sens);
float gyro_z = degrees_to_radian_conversion(gyro_z_dps);


Gyro.x = gyro_x;
Gyro.y = gyro_y;
Gyro.z = gyro_z;

return (Gyro);

}


//Magnetometer Auslesen

struct Vector read_magnetometer(int file){
Vector Mag;
int mag_x_h = read_register(file, r_mag_x_h);
int mag_x_l = read_register(file, r_mag_x_l);
int16_t mag_x_out = high_low_bytes_merge(mag_x_h, mag_x_l);
double mag_x = magnetometer_sensitivity_tesla(mag_x_out);


int mag_y_h = read_register(file, r_mag_y_h);
int mag_y_l = read_register(file, r_mag_y_l);
int mag_y_out = high_low_bytes_merge(mag_y_h, mag_y_l);
double mag_y = magnetometer_sensitivity_tesla(mag_y_out);


int mag_z_h = read_register(file, r_mag_z_h);
int mag_z_l = read_register(file, r_mag_z_l);
int mag_z_out = high_low_bytes_merge(mag_z_h, mag_z_l);
double mag_z = magnetometer_sensitivity_tesla(mag_z_out);


read_register(file, r_tmps);
read_register(file, r_st2);



Mag.x = mag_x;
Mag.y = mag_y;
Mag.z = mag_z;

return (Mag);

}



//Temperatur Auslesen

float read_temperature(int file){

int temp_h = read_register(file, r_temp_h);
int temp_l = read_register(file, r_temp_l);
int16_t temp_out = high_low_bytes_merge(temp_h, temp_l);
float temp = temperature_measure( temp_out);

return temp;

}

//Norm Berechnen
double calculate_norm (double x, double y, double z){

double norm = sqrt(x*x + y*y + z*z);

return norm;

}