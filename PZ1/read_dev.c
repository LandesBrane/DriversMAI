#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <устройство>\n", argv[0]);
        fprintf(stderr, "Пример: %s /dev/zero\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *device = argv[1];
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    unsigned char buf[100];
    ssize_t nread = read(fd, buf, sizeof(buf));
    if (nread == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Прочитано %zd байт из %s\n", nread, device);

    // Для /dev/zero байты будут нулевыми, для /dev/random — случайными.
    // Выведем первые 16 байт в шестнадцатеричном виде (для наглядности)
    printf("Первые байты (hex): ");
    for (ssize_t i = 0; i < nread && i < 16; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}