#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct headerRecord {
    char fileName[20];
    long start;
    long size;
};

int fileNamesStartIndex;
bool extract = false;
bool verbose = false;
bool create = false;
bool delete = false;
bool update = false;
bool append = false;
bool list = false;
bool pack = false;
bool file = false;
char *tarName;


void parameterChecker(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Faltan parametros\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if(argv[i][0]!='-') {
            fileNamesStartIndex = i;
            break;
        }
        if (argv[i][1] == '-') {
            if(strcmp(&argv[i][2],"create") == 0){
                if(create == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                create = true;
            } else if(strcmp(&argv[i][2],"extract") == 0) {
                if(extract == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                extract = true;
            } else if(strcmp(&argv[i][2],"list") == 0) {
                if(list == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                list = true;
            } else if(strcmp(&argv[i][2],"delete") == 0) {
                if(delete == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                delete= true;
            } else if(strcmp(&argv[i][2],"update") == 0) {
                if(update == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                update = true;
            } else if(strcmp(&argv[i][2],"verbose") == 0) {
                if(verbose == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                verbose = true;
            } else if(strcmp(&argv[i][2],"file") == 0) {
                if(file == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                file = true;
            } else if(strcmp(&argv[i][2],"append") == 0) {
                if(append == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                append = true;
            } else if(strcmp(&argv[i][2],"pack") == 0) {
                if(pack == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                pack = true;
            } else {
                printf("Parametro inexistente: %s\n", &argv[i][2]);
                exit(1);
            }
            continue;
        }

        for (long j = 1; j < strlen(argv[i]); j++) {
            if(argv[i][j] == 'c') {
                if(create == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                create = true;
            } else if(argv[i][j] == 'x') {
                if(extract == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                extract = true;
            } else if(argv[i][j] == 't') {
                if(list == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                list = true;
            } else if(argv[i][j] == 'u') {
                if(update == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                update = true;
            } else if(argv[i][j] == 'v') {
                if(verbose == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                verbose = true;
            } else if(argv[i][j] == 'f') {
                if(file == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                file = true;
            } else if(argv[i][j] == 'r') {
                if(append == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                append= true;
            } else if(argv[i][j] == 'p') {
                if(pack == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                pack = true;
            } else {
                printf("Parametro inexistente: %c\n", argv[i][j]);
                exit(1);
            }
        }
    }
    
    if(strcmp(&argv[fileNamesStartIndex][strlen(argv[fileNamesStartIndex])-5],".star") != 0 || strlen(argv[fileNamesStartIndex]) <= 5) {
        printf("El nombre debe terminar en .star y no ser solo la extension: %s\n", argv[fileNamesStartIndex]);
        exit(1);
    }
    tarName = argv[fileNamesStartIndex];
    fileNamesStartIndex++;
}

void createStar() {
    FILE *fp = fopen(tarName, "r");
    if(fp != NULL) {
        printf("El archivo ya existe\n");
        fclose(fp);
        exit(1);
    }
    fp = fopen(tarName, "w");



    fclose(fp);
}

int main(int argc, char *argv[]) {
    parameterChecker(argc,argv);
    /*
    printf("extract: %s\n", extract ? "true" : "false");
    printf("verbose: %s\n", verbose ? "true" : "false");
    printf("create: %s\n", create ? "true" : "false");
    printf("delete: %s\n", delete ? "true" : "false");
    printf("update: %s\n", update ? "true" : "false");
    printf("append: %s\n", append ? "true" : "false");
    printf("list: %s\n", list ? "true" : "false");
    printf("pack: %s\n", pack ? "true" : "false");
    printf("file: %s\n", file ? "true" : "false");
    */

    if(create) {
        createStar();
    }

    exit(0);
}