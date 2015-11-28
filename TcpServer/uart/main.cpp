#include <iostream>
#include "cppwrapper.h"
#include <string>
#include <thread>
#include <chrono>

int main()
{
    std::ios_base::sync_with_stdio(true);
	std::cout << "Hello world\n";
    gps::c_link::uart_conn conn;
    for(int i=0;i<10;++i){
        auto str = conn.read();
        std::cout << str << str.length() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(8));
    }
	return 0;	
}

