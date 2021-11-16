/*
    Filename - ServoWrite.cpp
    Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
    Date - 16/11/2021
*/

#include<cstdlib>
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>
#include<string.h>
#include<stdio.h>
using namespace std;

// The pwm path of the window servo
#define ANALOG_PATH "/sys/class/pwm/pwmchip1/pwm-1:1"

/*
    Name - write
    Purpose - To write a value into a specific file given a specific path
    Description of inputs - String representations of the path, filename and value
    Description of return values - Returns 0 if passed successfully and -1 and a perror if not
*/
int write(string path, string filename, string value)
{
    ofstream fs;
    fs.open((path + filename).c_str());
    if (!fs.is_open())
    {
        perror("GPIO: write failed to open file ");
        return -1;
    }
    fs << value;
    fs.close();
    return 0;
}

/*
    Name - write
    Purpose - Overloaded function that makes it easier to write a value to file
    Description of inputs - String representations of the path, filename and integer value
    Description of return values - Returns 0 if passed successfully and -1 and a perror if not
    as it only casts the integer into string
*/
int write(string path, string filename, int value)
{
    stringstream s;
    s << value;
    return write(path,filename,s.str());
}

/*
    Name - exec
    Purpose - To run a bash command and return the console return 
    Description of inputs - A character pointer array that represents the command
    Description of return values - Returns the result or throws a runtime_error if the pipe open fails or char buffer is full
*/
string exec(const char* cmd)
{
    char buffer[256];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    } catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

/*
    Name - validatePWM
    Purpose - To validate if the pin is exported and configured to be in PWM mode
    Description of inputs - There are none
    Description of return values - Returns 0 if passed successfully and -1
*/
int validatePWM()
{
    string s(exec("config-pin -q P9_21"));
    struct stat info;

    if( stat( ANALOG_PATH, &info ) != 0 )
    {
        printf( "cannot access %s\n", ANALOG_PATH);
        return -1;
    }
    else if( info.st_mode & S_IFDIR )
    {
        printf( "%s is a directory\n", ANALOG_PATH);
        if(strchr(s.c_str(),'w') == NULL)
        {
            system("config-pin P9_21 pwm");
            return 0;
        }
        else return 0;
    }
    else
    {
        printf( "%s is no directory\n", ANALOG_PATH);
        write("/sys/class/pwm/pwmchip1", "/export", 1);
        if(strchr(s.c_str(),'w') == NULL)
        {
            system("config-pin P9_21 pwm");
            return 0;
        }
        else return 0;
    }
    return 0;
}

/*
    Name - light
    Purpose - To initialize the period, duty cycle and then enable the PWM pin
    Description of inputs - The percentage of brightness to set 
    Description of return values - Returns -1 if not successful
*/
int light(int percent)
{
    if(validatePWM() != 0)
    {
        return -1;
    }

    int period = 20000000;
    int duty_cycle = (percent+1)*1000000;

    write(ANALOG_PATH, "/period", period);
    write(ANALOG_PATH, "/duty_cycle", duty_cycle);
    write(ANALOG_PATH, "/enable", 1);
}

int main(int argc, char* argv[])
{
    int input = stoi(argv[1]);
    if(input == 0)
        light(input);
    else light(1);
}
