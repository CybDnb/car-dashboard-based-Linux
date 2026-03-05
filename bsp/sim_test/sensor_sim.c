#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

const char fifo_path[] = "/tmp/sensor_fifo";

int main(int argc, char const *argv[])
{
    log_info("Starting sensor simulator...");
    mkfifo(fifo_path, 0666);
    int fd = open(fifo_path, O_WRONLY);
    if (fd < 0) {
        log_error("Failed to open FIFO for writing");
        return -1;
    }
    while(1){
        log_warn("Simulating sensor data...");
        write(fd, "Sensor data\n", 12);
        sleep(3);
    }
    return 0;
}
