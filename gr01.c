#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CONTEXTO 30

int buscar(const char *buffer, const char *word) {
    int tam_buffer = strlen(buffer);
    int tam_word = strlen(word);

    for (int i = 0; i <= tam_buffer - tam_word; i++) {
        if (strncmp(&buffer[i], word, tam_word) == 0) {
            return i;
        }
    }

    return -1;
}

void contexto(const char *line, int pos, int taman_word) {
    int start = (pos - CONTEXTO) > 0 ? (pos - CONTEXTO) : 0;
    int end = pos + taman_word + CONTEXTO;
    printf("...%.*s...\n", end - start, &line[start]);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <texto> <arquivo>\n", argv[0]);
        return 1;
    }

    const char *word = argv[1];
    const char *filename = argv[2];

    char buffer[BUFFER_SIZE];
    int fd, bytes_rd, numero_linha = 1;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    while ((bytes_rd = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_rd] = '\0';

        char *comeco_linha = buffer;
        for (int i = 0; i < bytes_rd; i++) {
            if (buffer[i] == '\n' || i == bytes_rd - 1) {
                buffer[i] = '\0';

                int pos = buscar(comeco_linha, word);
                if (pos != -1) {
                    printf("Palavra '%s' encontrada na linha %d, posição %d:\n", word, numero_linha, pos);
                    contexto(comeco_linha, pos, strlen(word));
                    //close(fd); 
                    //return 0;
                }

                comeco_linha = &buffer[i + 1];
                numero_linha++;
            }
            
        }
    }

    if (bytes_rd == -1) {
        printf("Erro ao ler o arquivo\n");
    } else {
        printf("Palavra '%s' não encontrada no arquivo.\n", word);
    }

    close(fd);
    return 0;
}
