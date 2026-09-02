#include "icm.hpp"
#include <iomanip>
#include <iostream>

using namespace std;

const int addr = 0x69;
__u8 pwr = 0x06;

//MAIN

int main() {

int datei = linux_i2c_file_open("/dev/i2c-1");


linux_i2c_bus_adress_access(datei, addr);
icm_cancel_sleep_mode(datei, pwr);

auto acc_sens = configure_accelerometer(datei);
auto gyro_sens = configure_gyroskop(datei);
configure_magnetometer(datei);




while (1){

//Accelerometer

Vector Acc = read_accelerometer(datei, acc_sens);
double acc_norm = calculate_norm(Acc.x, Acc.y, Acc.z);

//Gyroskop

Vector Gyro = read_gyroksop(datei, gyro_sens);


// Magnetometer
Vector Mag =  read_magnetometer(datei);
double mag_norm = calculate_norm(Mag.x, Mag.y, Mag.z);


// Temperatur
double temp = read_temperature(datei);


//Ausgabe 
//Accelerometer

cout << "Accelerometer X-Achse: " << Acc.x << " m/s² " << '\n';
cout << "Accelerometer Y-Achse: " << Acc.y << " m/s² " << '\n';
cout << "Accelerometer Z-Achse: " << Acc.z << " m/s² " << '\n';
cout << "Accelerometer Wurzel: " << acc_norm << " m/s² " << '\n';

//Gyroskop
cout << "Gyroskop X-Achse: " << Gyro.x <<  "rad/s" << '\n';
cout << "Gyroskop Y-Achse: " << Gyro.y <<  "rad/s" << '\n';
cout << "Gyroskop Z-Achse: " << Gyro.z <<  "rad/s" << '\n';

//Magnetometer
cout << "Magnetometer X-Achse: " << fixed << setprecision(6) << Mag.x << " T" << '\n'; 
cout << "Magnetometer Y-Achse: " << fixed << setprecision(6) << Mag.y << " T" << '\n'; 
cout << "Magnetometer Z-Achse: " << fixed << setprecision(6) << Mag.z << " T" << '\n'; 
cout << "Magnetometer Norm " << setprecision(6) << mag_norm << " T" << '\n';

//Temperatur

cout << "Temperatur" << ": " << temp << " °C" <<'\n';


}

return 0;

}