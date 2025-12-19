#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_PATH "/dev/timerdev"

#define TIMER_IOC_MAGIC 't'
#define TIMER_IOC_GET_HISTOGRAM _IO(TIMER_IOC_MAGIC, 0)

int main(int argc, char *argv[])
{
    int fd, i, n = 10;
    int32_t value;

    if (argc > 1)
        n = atoi(argv[1]);

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open " DEVICE_PATH);
        return 1;
    }

    printf("Performing %d write-read cycles...\n", n);

    for (i = 0; i < n; i++) {
        value = i + 1000;
        if (write(fd, &value, sizeof(value)) != sizeof(value)) {
            perror("write");
            break;
        }

        // Небольшая задержка (можно убрать)
        usleep(1000 + (i % 10) * 500);  // 1–6 мс

        if (read(fd, &value, sizeof(value)) != sizeof(value)) {
            perror("read");
            break;
        }
        printf("  Cycle %02d: wrote %d, read %d\n", i+1, i+1000, value);
    }

    printf("\nRequesting histogram via ioctl...\n");
    if (ioctl(fd, TIMER_IOC_GET_HISTOGRAM) < 0) {
        perror("ioctl");
    } else {
        printf("Histogram printed to kernel log (check dmesg)\n");
    }

    close(fd);
    return 0;
}