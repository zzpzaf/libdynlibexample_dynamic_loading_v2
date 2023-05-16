// utils.cpp

#include <string>
#include <string>
#include <sstream>



int getfirstinteger(std::string str) {

    std::stringstream ss ;
    std::string fword;
    int r, n;
    r = 0;

    ss << str;
    ss >> fword;
    if (std::stringstream(fword) >> n) {
        r=std::stoi(fword);
    }
    return r;
}