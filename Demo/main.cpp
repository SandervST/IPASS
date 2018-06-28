#include "hwlib.hpp"
#include "MPU6050.hpp"


int16_t accelX, accelY, accelZ;
const int speedsens = 1'000; //Sensitivity of the speed with the accelerometer(lower = faster)
uint8_t targetX, targetY;
const uint8_t targetsize = 1; //Size of the targets
const int ballsize = 3; //Size of the player
int score = 0;
uint64_t startTime, currentTime;
const int timer = 5'000'000; //Timer to reach the target. 1'000'000 == 1 second
const int restartGameSens = 30'000; //The value needed from the accelerometer Z-axis when game over to restart the game 

//Create new random x and y values for the target location  
void targetLocation(uint8_t & x, uint8_t & y){
    x = rand() % 123 + 5;
    y = rand() % 59 + 5;
}


int main( void ){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    namespace target = hwlib::target;
    
    //Start location for the player
    hwlib::location ballLocation(64,32);
    
    
    
    //create the I2C bus
    auto scl = target::pin_oc{ target::pins::scl };
    auto sda = target::pin_oc{ target::pins::sda };
    
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda{ scl,sda };
    
    auto display = hwlib::glcd_oled{ i2c_bus, 0x3c};
    
    //Startup the MPU6050
    mpu6050lib(i2c_bus).startup();
    
    //Create the first starting point for the target 
    //and save the start time is micro seconds for the timer.
    targetLocation(targetX, targetY);
    startTime = hwlib::now_us();
    
    for(;;){	
        
        display.clear(hwlib::buffering::buffered);
        
        //Get the accelerometer x and y value from the MPU6050
        accelX = mpu6050lib(i2c_bus).getAccelX();
        accelY = mpu6050lib(i2c_bus).getAccelY();
        
        //Get the current time is mirco seconds
        currentTime = hwlib::now_us();
        
        //Game over
        //Check if the current time passes the time at the start + timer(5 second) so it checks if 5 seconds have passed
        //If 5 second passed it's game over. 
        //When game over you can restart the game by quickly lifting the controller.
        if(currentTime >= (startTime + timer)){
            display.clear();
            hwlib::cout << "Game Over!" << "\n";
            for(unsigned int j = 0; j < display.size.y; j+=4){
                for( unsigned int i = 0; i < display.size.x; i++ ){
                    display.write( hwlib::location( i, j ), hwlib::black );
                }
            }
            hwlib::cout << "Quickly lift your controller to restart the game" << "\n";
            accelZ = mpu6050lib(i2c_bus).getAccelZ();
            while(accelZ < restartGameSens){
                accelZ = mpu6050lib(i2c_bus).getAccelZ();
            }
            display.clear();
            score = 0;                
            hwlib::cout << "Score: " << score << "\n";
            startTime = hwlib::now_us();
            targetLocation(targetX, targetY);
            ballLocation.x = 64;
            ballLocation.y = 32;
        }

        //Collision detection
        //This detects if the player hits the target
        //and if so the score goes 1 up, the target get a new location 
        //and the start time gets updates to reset the timer.
        if((ballLocation.y + ballsize) >= targetY 
        && (ballLocation.y - ballsize) <=targetY
        && (ballLocation.x + ballsize) >= targetX
        && (ballLocation.x - ballsize) <= targetX){
            score++;
            hwlib::cout << "Score: " << score << "\n";
            targetLocation(targetX, targetY);
            startTime = hwlib::now_us();
        }
    
        //Control the ball by dividing the raw value from the 
        //accelerometer with the set speedsensitivity.
        ballLocation.y -= accelX / speedsens;
        ballLocation.x -= accelY / speedsens;
    
        //Create the target location with the random x and y values
        hwlib::location randomlocation(targetX, targetY);
	
        //Create and draw the player ball and target on the display.
        hwlib::circle ball(ballLocation, ballsize);
        hwlib::circle target(randomlocation, targetsize);
        display.clear(hwlib::buffering::buffered);
        ball.draw(display); 
        target.draw(display);
        display.flush(); 
    }
}   