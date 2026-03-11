#include "gpio_turn.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char const *argv[])
{
    int fd = open("/dev/gpio_turn", O_RDWR);
    if (fd < 0) {   
        perror("Failed to open device");
        return EXIT_FAILURE;
    }
    unsigned long data = 0;
    data = 1000;
    ioctl(fd, GPIO_TURN_ALL_BLINK_START, &data);
    sleep(5);
    data = 1000;
    ioctl(fd, GPIO_TURN_ALL_BLINK_START, &data);
    sleep(5);
    data = 1000;
    ioctl(fd, GPIO_TURN_ALL_BLINK_START, &data);
    sleep(5); 
    data = 1000;
    ioctl(fd, GPIO_TURN_ALL_BLINK_START, &data);
    sleep(5);
    close(fd);
    return 0;
}
