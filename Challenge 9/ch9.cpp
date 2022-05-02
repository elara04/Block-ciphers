#include<iostream>
#include<iomanip>
#include<string>

int main()
{
    std::string txt{"YELLOW SUBMARINE"};
    char ch ='\\';
    std::string pad = "x04";
    // even multiple of 16 would be 32 so i added 4 hex charactrers to be 4 more bytes
    std::cout <<'"' << txt + ch + pad + ch + pad + ch + pad + ch + pad << '"' << std::endl;
    return 0;
}
