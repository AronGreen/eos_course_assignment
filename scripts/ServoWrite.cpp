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

#define ANALOG_PATH "/sys/class/pwm/pwmchip1/pwm-1:1"

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

int write(string path, string filename, int value)
{
    stringstream s;
    s << value;
    return write(path,filename,s.str());
}


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
