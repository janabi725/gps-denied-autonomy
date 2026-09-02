
//Bibliotheken
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>
#include <string>
#include <tuple>
#include <iomanip>
#include <string>


extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

using namespace std;


// Struktur Definitionen
struct Vector{
double x;
double y;
double z;};

//Funktion Deklaration

int linux_i2c_file_open(string str);
int linux_i2c_bus_adress_access(int file, int address);
int icm_cancel_sleep_mode(int file, int power_adress);
int configure_accelerometer(int file);
float configure_gyroskop(int file);
int configure_magnetometer(int file);
struct Vector read_accelerometer(int file, int acc_sens);
struct Vector read_gyroksop(int file, float gyro_sens);
struct Vector read_magnetometer( int file);
float read_temperature(int file);
double calculate_norm (double x, double y, double z);


