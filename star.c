#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

struct headerRecord {
    char fileName[20];
    long start;
    long size;
    bool deleted;
};

#define HEADEREND sizeof(struct headerRecord)*101

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
    if(fileNamesStartIndex >= argc) {
        printf("No hay nombre para el tar\n");
        exit(1);
    }
    if(strcmp(&argv[fileNamesStartIndex][strlen(argv[fileNamesStartIndex])-5],".star") != 0 || strlen(argv[fileNamesStartIndex]) <= 5) {
        printf("El nombre debe terminar en .star y no ser solo la extension: %s\n", argv[fileNamesStartIndex]);
        exit(1);
    }
    tarName = argv[fileNamesStartIndex];
    fileNamesStartIndex++;
}

void createStar(int parameterCount, char *parameters[]) {
    char *buffer;
    FILE *fileToAdd;
    int recordCount = 0;
    int lastFilePosition = HEADEREND;
    struct headerRecord fileHeaderRecord;

    // Verificamos si existe el archivo porque significa que no podemos crearlo
    FILE *tarFile = fopen(tarName, "r");
    if(tarFile != NULL) {
        printf("El archivo ya existe\n");
        fclose(tarFile);
        exit(1);
    }

    // En un futuro deberiamos poder recibir archivos desde stdin, por ahora
    // ocupamos el parametro -f para que funcione
    if(!file) {
        printf("stdin not implemented yet");
        exit(1);
    }

    tarFile = fopen(tarName, "w"); // creamos el archivo

    // Un ciclo que va a revisar cada archivo indicado en los parametros
    for(int i = fileNamesStartIndex ; i < parameterCount ; i++) {

        // Si el nombre del archivo es superior a 20 caracteres no podemos empaquetar ese archivo y pasamos al siguiente
        if(strlen(parameters[i]) > 20) {
            if(verbose) {
                printf("El archivo: %s tiene un nombre mayor a 20 caracteres\n", parameters[i]);
            }
            continue;
        }

        if(verbose) {
            printf("Intentando abrir el archivo: %s\n", parameters[i]);
        }

        // Abrimos el archivo a empaquetar
        fileToAdd = fopen(parameters[i], "r"); 
        // Si no existe pasamos al siguiente
        if(tarFile == NULL) {
            if(verbose) {
                printf("El archivo: %s no existe, continuando con el siguiente\n", parameters[i]);
            }
            continue;
        }

        if(verbose) {
            printf("Se logro abrir el archivo: %s\n", parameters[i]);
        }
        
        strcpy(fileHeaderRecord.fileName,parameters[i]); // Guardamos el nombre del archivo
        fileHeaderRecord.start = lastFilePosition; // Guardamos la posicion inicial del archivo
        fileHeaderRecord.deleted = false; // Decimos que no esta borrado

        // Aqui buscamos de que tamaño es el archivo
        fseek(fileToAdd, 0, SEEK_END);
        fileHeaderRecord.size = ftell(fileToAdd);
        rewind(fileToAdd); 
        
        // Buscamos la posicion donde debemos guardar el nuevo registro en el header
        fseek(tarFile, (sizeof(fileHeaderRecord)*recordCount)+sizeof(int), SEEK_SET);
        fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
        
        // Creamos un espacio en memoria del tamaño del archivo a empaquetar
        buffer = (char *)malloc(fileHeaderRecord.size);
        fread(buffer, fileHeaderRecord.size, 1, fileToAdd); // cargamos el archivo a memoria

        // Vamos a donde se va a guardar el archivo
        fseek(tarFile, lastFilePosition, SEEK_SET);
        fwrite(buffer,fileHeaderRecord.size,1,tarFile); // lo guardamos

        if(verbose) {
            printf("Se empaqueto el archivo: %s\n", parameters[i]);
        }
        
        lastFilePosition = ftell(tarFile); // La posicion del siguiente archivo es donde quedo la escritura del otro

        // liberamos memoria
        free(buffer);
        fclose(fileToAdd);

        // Sumamos un registro de archivo
        recordCount++;
    }
    
    // Guardamos la cantidad de registros al inicio del header
    rewind(tarFile); 
    fwrite(&recordCount, sizeof(int), 1, tarFile);

    // cerramos el tar
    fclose(tarFile);
}

void extractStar(int parameterCount, char *parameters[]) {
    char *buffer;
    int recordCount;
    FILE *currentExtractFile;
    struct headerRecord fileHeaderRecord;

    // Abrir el archivo, si no existe se da error.
    // Si no existiera no hay que extraer
    FILE *tarFile = fopen(tarName, "r");
    if(tarFile == NULL) {
        printf("El archivo no existe\n");
        fclose(tarFile);
        exit(1);
    }

    // Lo primero que se escribe en el header es la cantidad de registros que tiene para
    // poder saber si hay archivos, y si hay cuantos son para recorrer todos los registros
    fread(&recordCount,sizeof(int),1,tarFile);
    

    // Si tiene 0 registros significa que no hay nada para extraer
    if(recordCount <= 0) {
        printf("El star no tiene archivos empaquetados\n");
        fclose(tarFile);
        exit(1);
    }

    // Con este ciclo vamos extrayendo uno por uno los archivos que hay segun el header
    for(int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // vamos al primer registro
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile); // obtenemos el registro
        
        // si el registro actual es de un archivo borrado, nos lo saltamos
        if(fileHeaderRecord.deleted) {
            continue;
        }

        //Crear archivo
        currentExtractFile = fopen(fileHeaderRecord.fileName, "w");

        //Extraer archivo
        buffer = (char *)malloc(fileHeaderRecord.size); // hacemos un espacio donde podamos cargar el archivo a memoria
        fseek(tarFile,fileHeaderRecord.start,SEEK_SET); // buscamos el archivo
        fread(buffer,fileHeaderRecord.size,1,tarFile);  // lo cargamos a memoria

        //Escribir archivo
        fwrite(buffer,fileHeaderRecord.size,1,currentExtractFile); // Escribimos el archivo cargado en memoria
                                                                   // en el nuevo archivo creado

        //liberar recursos
        free(buffer);
        fclose(currentExtractFile);
    }

    fclose(tarFile);
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
        createStar(argc,argv);
    } else if(extract) {
        
        extractStar(argc,argv);
    }

    exit(0);
}