// ==============================================================
//
// Copyright Sander van Sterkenburg 2018
//      
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==============================================================

#include "hwlib.hpp"
#include "MPU6050.hpp"

int counter = 0;
char i;
int16_t gyroX, gyroY, gyroZ, accelX, accelY, accelZ;

int main( void ){	
    
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   auto display = hwlib::glcd_oled( i2c_bus, 0x3c );  
    

   display.clear();
   
   for(unsigned int j = 0; j < display.size.y; j++){
        for( unsigned int i = 0; i < display.size.x; i++ ){
            display.write( hwlib::location( i, j ), hwlib::black );
        }
   }
   display.clear();
   
    for(unsigned int j = 0; j < display.size.y; j+=4){
        for( unsigned int i = 0; i < display.size.x; i++ ){
            display.write( hwlib::location( i, j ), hwlib::black );
        }
   }
   display.clear();
   
   while(counter < 60){
       counter++;
       hwlib::location randomlocation(rand() % 123 + 5, rand() % 59 + 5);
       hwlib::circle target(randomlocation, rand() % 10 + 1);
       target.draw(display);
   }
   display.clear();
   
   hwlib::cout << "Display test done." << "\n" << "\n";
   
   
   mpu6050lib(i2c_bus).testVariables();
   
   hwlib::cout << "\nPlace the controller on a flat surface and press enter." << "\n";
   hwlib::cin >> i;
   mpu6050lib(i2c_bus).startup();
   gyroX = mpu6050lib(i2c_bus).getGyroX();
   gyroY = mpu6050lib(i2c_bus).getGyroY();
   gyroZ = mpu6050lib(i2c_bus).getGyroZ();
   
   accelX = mpu6050lib(i2c_bus).getAccelX();
   accelY = mpu6050lib(i2c_bus).getAccelY();
   accelZ = mpu6050lib(i2c_bus).getAccelZ();
   
   hwlib::cout << "Gyro x: " << gyroX << " Gyro y: " << gyroY << " Gyro z: " << gyroZ << "\n";
   
   if(gyroX < 4 && gyroX > -4 
   && gyroY < 4 && gyroY > -4 
   && gyroZ < 4 && gyroZ > -4 ){
       hwlib::cout << "\nTest is ok" << "\n";
   }
   else{
        hwlib::cout << "\nTest is NOT ok" << "\n";
   }
   
   hwlib::cout << "\nAccel x: " << accelX << " Accel y: " << accelY << " Accel z: " << accelZ << "\n";
   
   if(accelX < 750 && accelX > -750
   && accelY < 750 && accelY > -750
   && accelZ < 19000 && accelZ > 16000){
       hwlib::cout << "\nTest is ok" << "\n";
   }
   else{
        hwlib::cout << "\nTest is NOT ok" << "\n";
   }
   
   hwlib::cout << "\nPlace the controller with the FRONT side facing down and press enter." << "\n";
   hwlib::cin >> i;

   accelX = mpu6050lib(i2c_bus).getAccelX();
   accelY = mpu6050lib(i2c_bus).getAccelY();
   accelZ = mpu6050lib(i2c_bus).getAccelZ();
   
   hwlib::cout << "Accel x: " << accelX << " Accel y: " << accelY << " Accel z: " << accelZ << "\n";
   
   if(accelX < -15000 && accelX > -16500
   && accelY < 750 && accelY > -750
   && accelZ < 1500 && accelZ > -0){
       hwlib::cout << "\nTest is ok" << "\n";
   }
   else{
        hwlib::cout << "\nTest is NOT ok" << "\n";
   }   
   
   hwlib::cout << "\nPlace the controller with the RIGHT side facing down and press enter." << "\n";
   hwlib::cin >> i;

   accelX = mpu6050lib(i2c_bus).getAccelX();
   accelY = mpu6050lib(i2c_bus).getAccelY();
   accelZ = mpu6050lib(i2c_bus).getAccelZ();
   
   hwlib::cout << "Accel x: " << accelX << " Accel y: " << accelY << " Accel z: " << accelZ << "\n";
   
   if(accelX < 750 && accelX > -750
   && accelY > -17000 && accelY < -15500
   && accelZ < 2500 && accelZ > 1000){
       hwlib::cout << "\nTest is ok" << "\n";
   }
   else{
        hwlib::cout << "\nTest is NOT ok" << "\n";
   }   
   
   hwlib::cout << "\nPlace the controller with the LEFT side facing down and press enter." << "\n";
   hwlib::cin >> i;

   accelX = mpu6050lib(i2c_bus).getAccelX();
   accelY = mpu6050lib(i2c_bus).getAccelY();
   accelZ = mpu6050lib(i2c_bus).getAccelZ();
   
   hwlib::cout << "Accel x: " << accelX << " Accel y: " << accelY << " Accel z: " << accelZ << "\n";
   
   if(accelX < 750 && accelX > -750
   && accelY < 17500 && accelY > 15500
   && accelZ < 2500 && accelZ > 1000){
       hwlib::cout << "\nTest is ok" << "\n";
   }
   else{
        hwlib::cout << "\nTest is NOT ok" << "\n";
   }
   
   hwlib::cout << "\nTest Completed!" << "\n";
}
 
