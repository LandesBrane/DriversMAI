#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/mydevice"

int main() {
    int fd;
    char write_buf[] = "Hello from user space!\n";
    char read_buf[256];
    ssize_t bytes;

    // Открытие устройства
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Запись
    bytes = write(fd, write_buf, strlen(write_buf));
    if (bytes < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    printf("Wrote %zd bytes\n", bytes);

    // Чтение
    lseek(fd, 0, SEEK_SET); // вернуться в начало
    bytes = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    read_buf[bytes] = '\0';
    printf("Read: %s", read_buf);

    close(fd);
    return 0;
}