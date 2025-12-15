#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

#define DEVICE_PATH "/dev/myprocdev"

#define IOCTL_MAGIC 'k'
#define IOCTL_RESET_BUFFER      _IO(IOCTL_MAGIC, 0)
#define IOCTL_GET_BUFFER_STATUS _IOR(IOCTL_MAGIC, 1, int)

int main() {
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open /dev/myprocdev");
        return 1;
    }

    // Запись данных
    const char *msg = "Hello from user space!";
    write(fd, msg, strlen(msg));
    printf("Wrote: %s\n", msg);

    // Проверка статуса через ioctl
    int status;
    if (ioctl(fd, IOCTL_GET_BUFFER_STATUS, &status) == 0) {
        printf("Buffer status: %s\n", status ? "full" : "empty");
    }

    // Чтение через /proc
    printf("\n=== /proc status ===\n");
    system("cat /proc/myproc_status");

    // Сброс буфера
    ioctl(fd, IOCTL_RESET_BUFFER, 0);
    printf("\nBuffer reset via ioctl.\n");

    // Проверка после сброса
    if (ioctl(fd, IOCTL_GET_BUFFER_STATUS, &status) == 0) {
        printf("Buffer status after reset: %s\n", status ? "full" : "empty");
    }

    printf("\n=== /proc status after reset ===\n");
    system("cat /proc/myproc_status");

    close(fd);
    return 0;
}