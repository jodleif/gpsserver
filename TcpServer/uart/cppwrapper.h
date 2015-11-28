#ifndef CPPWRAPPER_H
#define CPPWRAPPER_H
#include <cstdint>
#include <string>
#include <stdexcept>
namespace gps {
namespace c_link {
class uart_conn {
      private:
        static const size_t bufflen = 1024u;
        unsigned char data[bufflen];
        int handle;

      public:
        uart_conn();
        ~uart_conn();
        std::string read();
};
}
}

#endif // CPPWRAPPER_H
