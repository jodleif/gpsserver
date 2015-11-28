#include <stdexcept>
#include <cassert>
#include "cppwrapper.h"
namespace gps {
namespace c_link {
extern "C" {
   #include "uart.h"
}
gps::c_link::uart_conn::uart_conn() {
    handle = setup_uartconn();
    if(handle==-1) throw std::runtime_error("Error initializing UART_connection to gps");
}

gps::c_link::uart_conn::~uart_conn() {
    if(handle!=-1){
    close_uartconn(handle);
    }
}

std::string gps::c_link::uart_conn::read() {
        std::string res;
        int len = read_uart(handle, &data[0], bufflen);
        if (len == -1)
                throw std::runtime_error("Could not read UART!");
        assert(len>=0);
        res.reserve(static_cast<size_t>(len));
        for (int i = 0; i < len; ++i) {
            res.push_back(data[i]);
        }
        return res;
}
}
}
