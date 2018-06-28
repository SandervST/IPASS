// ==============================================================
//
// Copyright Sander van Sterkenburg 2018
//      
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==============================================================

#ifndef MPU6050_HPP
#define MPU6050_HPP
#include "hwlib.hpp"

/// @file

/// \brief
/// MPU6050 library
/// \details
/// This library creates an easy way to read the data from the MPU6050
/// accelerometer, gyroscope and temperatur registers.
    
class mpu6050lib{
private:
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    
    //This is the address of the MPU6050 when AD0 is set to low
    //, When set to high the addres is 0x69
    uint8_t address = 0x68;
    
    //These are the accelerometer register addresses
    const uint8_t ACCEL_XOUT = 0x3B;
    const uint8_t ACCEL_YOUT = 0x3D;
    const uint8_t ACCEL_ZOUT = 0x3F;    
    
    //This is the temperatur register address
    const uint8_t TEMP_OUT = 0x41;
    
    //These are the gyroscope register addresses
    const uint8_t GYRO_XOUT = 0x43;
    const uint8_t GYRO_YOUT = 0x45;
    const uint8_t GYRO_ZOUT = 0x47;
    
    //These are the default sensitivity when both config registers are set to 0.
    const int gyroSens = 131;
    const int accelSens = 16384;
    
    int16_t accelX, accelY, accelZ;
    int16_t gyroX, gyroY, gyroZ;
    int16_t temperature;
    
public:
    /// \brief
    /// Default constructor.
    /// \details
    /// The constructor of the mpu6050lib with one parameter for the I2C bus.
    mpu6050lib(hwlib::i2c_bus_bit_banged_scl_sda bus):
        bus(bus)
        {}
        
    /// \brief
    /// Wake up the MPU6050.
    /// \details
    /// This function writes a 0 to the power management regtister 
    /// to disable, the default enabled, sleep mode. 
    void startup(){
        uint8_t data[2] = {0x6B, 0};
        bus.write(address, data, 2);
        configMPU();
    }
    
    /// \brief
    /// Configurates MPU6050.
    /// \details
    /// This Funtions writes a 0 to both the gyroscope and accelerometer 
    /// configuration register. To set the gyro to full scale +/- 250deg/s
    /// and the accel to +/- 2g.
    void configMPU(){
        uint8_t dataGyro[2] = {0x1B, 0x00};
        bus.write(address, dataGyro, 2);
        uint8_t dataAccel[2] = {0x1C, 0x00};
        bus.write(address, dataAccel, 2);     
    }
    
    /// \brief
    /// Read the accelerometer X value.
    /// \details
    /// This function extracts the raw value of the accelerometer 
    /// X-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default this funtion returns the raw value accelerometer.
    /// To return the value in g's uncomment the second to last 
    /// sentence of this function.
    int16_t getAccelX(){
        uint8_t data[2] = {ACCEL_XOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        accelX = (data[0] << 8) + data[1];
        //accelX = accelX / accelSens;
        return accelX;
    }
        
    /// \brief
    /// Read the accelerometer Y value.
    /// \details
    /// This function extracts the raw value of the accelerometer 
    /// Y-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default returns this funtion the raw value accelerometer.
    /// To return the value in g's uncomment the second to last 
    /// sentence of this function.   
    int16_t getAccelY(){
        uint8_t data[2] = {ACCEL_YOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        accelY = (data[0] << 8) + data[1];
        //accelY = accelY / accelSens;
        return accelY;
    }
    
    /// \brief
    /// Read the accelerometer Z value.
    /// \details
    /// This function extracts the raw value of the accelerometer 
    /// Z-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default returns this funtion the raw value accelerometer.
    /// To return the value in g's uncomment the second to last 
    /// sentence of this function.
    int16_t getAccelZ(){
        uint8_t data[2] = {ACCEL_ZOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        accelZ = (data[0] << 8) + data[1];
        //accelZ = accelZ / accelSens;
        return accelZ;
    }
    

    /// \brief
    /// Read the gyroscope X value.
    /// \details
    /// This function extracts the raw value of the gyroscope 
    /// X-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default returns this funtion the value in degrees/s.
    /// To return the raw value comment out the second to last 
    /// sentence of this function.
    int16_t getGyroX(){
        uint8_t data[2] = {GYRO_XOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        gyroX = (data[0] << 8) + data[1];
        gyroX = gyroX / gyroSens;
        return gyroX;
    }
    
    /// \brief
    /// Read the gyroscope Y value.
    /// \details
    /// This function extracts the raw value of the gyroscope 
    /// Y-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default returns this funtion the value in degrees/s.
    /// To return the raw value comment out the second to last 
    /// sentence of this function.
    int16_t getGyroY(){
        uint8_t data[2] = {GYRO_YOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        gyroY = (data[0] << 8) + data[1];
        gyroY = gyroY / gyroSens;
        return gyroY;
    }
    
    /// \brief
    /// Read the gyroscope Z value.
    /// \details
    /// This function extracts the raw value of the gyroscope 
    /// Z-axis by reading two bytes and shifting them into a 16-bit integer.
    /// On default returns this funtion the value in degrees/s.
    /// To return the raw value comment out the second to last 
    /// sentence of this function.
    int16_t getGyroZ(){
        uint8_t data[2] = {GYRO_ZOUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        gyroZ = (data[0] << 8) + data[1];
        gyroZ = gyroZ / gyroSens;
        return gyroZ;
    }

    /// \brief
    /// Read the temperature value.
    /// \details
    /// This function extacts the raw value of the temperature
    /// by reading two bytes and shifting them into a 16-bit integer.
    /// On default this function returns the value in degrees Celsius.
    /// To return the raw value comment out the second to last 
    /// sentence of this function.
    int16_t getTemp(){
        uint8_t data[2] = {TEMP_OUT};
        bus.write(address, data, 1);
        bus.read(address, data, 2);
        temperature = (data[0] << 8) + data[1];
        temperature = temperature/ 340 + 36.53;
        return temperature;
    }
    
    /// \brief
    /// Test function
    /// \details
    /// This function can be called to check if the set variables are still ok.
    void testVariables(){
        if(address == 0x68
        && ACCEL_XOUT == 0x3B
        && ACCEL_YOUT == 0x3D
        && ACCEL_ZOUT == 0x3F
        && TEMP_OUT == 0x41
        && GYRO_XOUT == 0x43
        && GYRO_YOUT == 0x45
        && GYRO_ZOUT == 0x47
        && gyroSens == 131
        && accelSens == 16384){
            hwlib::cout << "Variables ok" << "\n";
        }
        else{
            hwlib::cout << "Variables NOT ok" << "\n";
        }
    }
    
    
};
    
#endif // MPU6050_HPP