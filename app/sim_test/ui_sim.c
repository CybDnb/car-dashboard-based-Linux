#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

const char fifo_path[] = "/tmp/sensor_fifo";

int main(int argc, char const *argv[])
{
    log_info("Starting ui simulator...");
    int fd = open(fifo_path, O_RDONLY);
    if (fd < 0) {
        log_error("Failed to open FIFO for reading");
        return -1;  
    }
    char buffer[128];
    while(1){
        int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            log_info("Received sensor data: %s", buffer);
        } else {
            log_error("Failed to read from FIFO");
            break;
        }
    }
    return 0;
}
