#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

struct headerRecord {
    char fileName[20];
    long start;
    long size;
    bool deleted;
};

#define HEADEREND sizeof(int)+sizeof(struct headerRecord)*101

int fileNamesStartIndex;
bool extract = false;
bool verbose = false;
bool vverbose = false;
bool create = false;
bool delete = false;
bool update = false;
bool append = false;
bool list = false;
bool pack = false;
bool file = false;
char *tarName;

void multipleOpParameterError() {
    if(create || extract || delete || update || append || list || pack) {
        printf("Multiples parametros de operacion no esta permitido\n");
        exit(1);
    } 
}

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
                multipleOpParameterError();
                create = true;
            } else if(strcmp(&argv[i][2],"extract") == 0) {
                if(extract == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                multipleOpParameterError();
                extract = true;
            } else if(strcmp(&argv[i][2],"list") == 0) {
                if(list == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                multipleOpParameterError();
                list = true;
            } else if(strcmp(&argv[i][2],"delete") == 0) {
                if(delete == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                multipleOpParameterError();
                delete= true;
            } else if(strcmp(&argv[i][2],"update") == 0) {
                if(update == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                multipleOpParameterError();
                update = true;
            } else if(strcmp(&argv[i][2],"verbose") == 0) {
                if(verbose == true && vverbose == true ) {
                    printf("Parametro repetido mas de 2 veces: %s\n", &argv[i][2]);
                    exit(1);
                } 
                if (verbose) {
                    vverbose  = true;
                } else {
                    verbose = true;
                }
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
                multipleOpParameterError();
                append = true;
            } else if(strcmp(&argv[i][2],"pack") == 0) {
                if(pack == true) {
                    printf("Parametro repetido: %s\n", &argv[i][2]);
                    exit(1);
                }
                multipleOpParameterError();
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
                multipleOpParameterError();
                create = true;
            } else if(argv[i][j] == 'x') {
                if(extract == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                multipleOpParameterError();
                extract = true;
            } else if(argv[i][j] == 't') {
                if(list == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                multipleOpParameterError();
                list = true;
            } else if(argv[i][j] == 'u') {
                if(update == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                multipleOpParameterError();
                update = true;
            } else if(argv[i][j] == 'v') {
                if(verbose == true && vverbose == true) {
                    printf("Parametro repetido mas de 2 veces: %c\n", argv[i][j]);
                    exit(1);
                } 
                if (verbose) {
                    vverbose  = true;
                } else {
                    verbose = true;
                }
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
                multipleOpParameterError();
                append= true;
            } else if(argv[i][j] == 'p') {
                if(pack == true) {
                    printf("Parametro repetido: %c\n", argv[i][j]);
                    exit(1);
                }
                multipleOpParameterError();
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


/*
Funcion para imprimir los verbose del create
*/
void verboseCreate(int comment, char *fileName) {
    if (verbose) {
        switch(comment) {
            case 1:
                printf("Intentando abrir el archivo: %s\n", fileName);
                break;
            case 2:
                printf("El archivo: %s no existe, continuando con el siguiente\n", fileName);
                break;
            case 3:
                printf("Se logro abrir el archivo: %s\n", fileName);
                break;
            case 4:
                printf("Se empaqueto el archivo: %s\n", fileName);
                break;
            default:
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 5:
                printf("El archivo: %s tiene un nombre mayor a 20 caracteres, continuando con el siguiente\n", fileName); 
                break;
            case 6:
                printf("El archivo: %s ya existe, continuando con el siguiente\n", fileName); 
                break;
            case 7:
                printf("Tamaño del archivo obtenido\n"); 
                break;
            case 8:
                printf("Registro del archivo guardado en el header\n"); 
                break;
            case 9:
                printf("Archivo cargado en memoria\n");
                break;
            case 10:
                printf("Liberando memoria...\n");
                break;
            case 11:
                printf("Guardando la cantidad de registros en el archivo...\n");
                break;
            default:
                break;
        } 
    }
}

/*
Funcion que revisa si este nombre de archivo ya se encuentra empaquetado
*/
bool checkDuplicate(FILE *tarFile, char *fileName[], int *recordCount) {
    for (int i = 0; i < *recordCount; i++) {
        //Obtener el header del archivo actual
        struct headerRecord record;
        fseek(tarFile, sizeof(int) + (sizeof(record) * i), SEEK_SET); // vamos al registro
        fread(&record, sizeof(record), 1, tarFile); // obtenemos el registro

        // Si los nombres coinciden y el registro no está marcado como eliminado, se retorna que está duplicado
        if (!record.deleted && strcmp(record.fileName, fileName[0]) == 0) {
            return true;
        }
    }
    return false; // No se encontraron duplicados
}

// Funcion para crear el star
void createStar(int parameterCount, char *parameters[]) {
    char *buffer;
    FILE *fileToAdd;
    int recordCount = 0;
    int lastFilePosition = HEADEREND;
    struct headerRecord fileHeaderRecord;
    
    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    // Verificamos si existe el archivo porque significa que no podemos crearlo
    FILE *tarFile = fopen(tarName, "r");
    if(tarFile != NULL) {
        printf("El archivo ya existe\n");
        fclose(tarFile);
        exit(1);
    }

    tarFile = fopen(tarName, "w"); // creamos el archivo

    // Un ciclo que va a revisar cada archivo indicado en los parametros
    for(int i = fileNamesStartIndex ; i < parameterCount ; i++) {

        // Si el nombre del archivo es superior a 20 caracteres no podemos empaquetar ese archivo y pasamos al siguiente
        if(strlen(parameters[i]) > 20) {
            verboseCreate(5,parameters[i]);
            continue;
        }

        if(checkDuplicate(tarFile,&parameters[i],&recordCount)) {
            verboseCreate(6,parameters[i]);
            continue;
        }

        verboseCreate(1,parameters[i]);

        // Abrimos el archivo a empaquetar
        fileToAdd = fopen(parameters[i], "r"); 
        // Si no existe pasamos al siguiente
        if(fileToAdd == NULL) {
            verboseCreate(2,parameters[i]);
            continue;
        }

        verboseCreate(3,parameters[i]);
        
        strcpy(fileHeaderRecord.fileName,parameters[i]); // Guardamos el nombre del archivo
        fileHeaderRecord.start = lastFilePosition; // Guardamos la posicion inicial del archivo
        fileHeaderRecord.deleted = false; // Decimos que no esta borrado

        // Aqui buscamos de que tamaño es el archivo
        fseek(fileToAdd, 0, SEEK_END);
        fileHeaderRecord.size = ftell(fileToAdd);
        rewind(fileToAdd); 
        
        verboseCreate(7,NULL);

        // Buscamos la posicion donde debemos guardar el nuevo registro en el header
        fseek(tarFile, (sizeof(fileHeaderRecord)*recordCount)+sizeof(int), SEEK_SET);
        fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

        verboseCreate(8,NULL);
        
        // Creamos un espacio en memoria del tamaño del archivo a empaquetar
        buffer = (char *)malloc(fileHeaderRecord.size);
        fread(buffer, fileHeaderRecord.size, 1, fileToAdd); // cargamos el archivo a memoria

        verboseCreate(9,NULL);

        // Vamos a donde se va a guardar el archivo
        fseek(tarFile, lastFilePosition, SEEK_SET);
        fwrite(buffer,fileHeaderRecord.size,1,tarFile); // lo guardamos

        verboseCreate(4,parameters[i]);
        
        lastFilePosition = ftell(tarFile); // La posicion del siguiente archivo es donde quedo la escritura del otro

        verboseCreate(10,NULL);
        // liberamos memoria
        free(buffer);
        fclose(fileToAdd);

        // Sumamos un registro de archivo
        recordCount++;
    }
    
    verboseCreate(11,NULL);
    // Guardamos la cantidad de registros al inicio del header
    rewind(tarFile); 
    fwrite(&recordCount, sizeof(int), 1, tarFile);

    // cerramos el tar
    fclose(tarFile);
}

/*
Funcion para imprimir los verbose del extract
*/
void verboseExtract(int comment, char *fileName) {
    if (verbose) {
        switch(comment) {
            case 1:
                printf("Creando archivo: %s\n", fileName);
                break;
            case 2:
                printf("Archivo: %s desempaquetado de forma correcta\n", fileName);
                break;
            default:
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 3:
                printf("Obteniendo registro de archivos...\n"); 
                break;
            case 4:
                printf("Registro obtenido es de un espacio vacio, ignorando...\n"); 
                break;
            case 5:
                printf("Registro obtenido: %s\n", fileName); 
                break;
            case 6:
                printf("Cargando a memoria datos del archivo: %s\n", fileName); 
                break;
            case 7:
                printf("Desempaquetando datos del archivo: %s\n", fileName);
                break;
            case 8:
                printf("Buscando si el archivo requiere extraerse...\n");
                break;
            case 9:
                printf("El archivo requiere extraerse\n");
                break;
            case 10:
                printf("El archivo NO requiere extraerse\n");
                break;
            default:
                break;
        } 
    }
}

bool isExtractNeeded(int parameterCount, char *parameters[], char *fileName) {
    verboseExtract(8,NULL);
    // Si no hay archivos especificados entonces hay que extraer todos
    if(fileNamesStartIndex >= parameterCount)
        return true;
    // Buscamos a ver si el nombre del registro actual coincide con uno de la lista de
    // archivos indicado en los parametros del comando
    for(int i = fileNamesStartIndex ; i < parameterCount ; i++) {
        if(strcmp(fileName,parameters[i]) == 0) {
            return true;
        }
    }
    return false;
}

void extractStar(int parameterCount, char *parameters[]) {
    char *buffer;
    int recordCount;
    FILE *currentExtractFile;
    struct headerRecord fileHeaderRecord;

    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    // Abrir el archivo, si no existe se da error.
    // Si no existiera no hay que extraer
    FILE *tarFile = fopen(tarName, "r");
    if(tarFile == NULL) {
        printf("El archivo no existe\n");
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
        verboseExtract(3,NULL);
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // vamos al primer registro
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile); // obtenemos el registro
        
        // si el registro actual es de un archivo borrado, nos lo saltamos
        if(fileHeaderRecord.deleted) {
            verboseExtract(4,NULL);
            continue;
        }
        verboseExtract(5,fileHeaderRecord.fileName);

        if(!isExtractNeeded(parameterCount,parameters,fileHeaderRecord.fileName)) {
            verboseExtract(10,NULL);
            continue;
        }
        verboseExtract(9,NULL);

        verboseExtract(1, fileHeaderRecord.fileName);
        //Crear archivo
        currentExtractFile = fopen(fileHeaderRecord.fileName, "w");

        verboseExtract(6, fileHeaderRecord.fileName);
        //Extraer archivo
        buffer = (char *)malloc(fileHeaderRecord.size); // hacemos un espacio donde podamos cargar el archivo a memoria
        fseek(tarFile,fileHeaderRecord.start,SEEK_SET); // buscamos el archivo
        fread(buffer,fileHeaderRecord.size,1,tarFile);  // lo cargamos a memoria

        verboseExtract(7, fileHeaderRecord.fileName);
        //Escribir archivo
        fwrite(buffer,fileHeaderRecord.size,1,currentExtractFile); // Escribimos el archivo cargado en memoria
                                                                   // en el nuevo archivo creado

        verboseExtract(2, fileHeaderRecord.fileName);

        verboseCreate(10,NULL);
        //liberar recursos
        free(buffer);
        fclose(currentExtractFile);
    }

    fclose(tarFile);
}


/*
Funcion para imprimir los verbose simples segun sea el caso 
*/
void verboseDelete(int comment, char *fileName[]) {
    if (verbose || vverbose) {
        switch(comment) {
            case 1:
                printf("Intentando abrir el archivo: %s\n", fileName[0]); 
                break;
            case 2:
                printf("Se logró abrir el archivo: %s\n", fileName[0]); 
                break;
            case 3:
                printf("Se eliminó el archivo: %s\n", fileName[0]);
                break;
            default:
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 4:
                printf("El registro anterior al registro que se va a eliminar esta marcado como eliminado\n"); 
                break;
            case 5:
                printf("El registro posterior al registro que se va a eliminar esta marcado como eliminado\n"); 
                break;
            case 6:
                printf("El registro se encuentra entre dos registros que estan marcados como eliminados\n");
                break;
            case 7:
                printf("Buscando registros vecinos que hayan sido eliminados\n");
                break;
            case 8:
                printf("Se aumentó el espacio disponible\n");
                break;
            case 9:
                printf("Se corrieron los registros del header\n");
                break;
            case 10:
                printf("No tiene vecinos eliminados\n");
                break;
            default:
                break;
        } 
    }
}

/*
Funcion encargada de realizar el corrimiento de los registros cuando se realiza un delete
*/
void moveRegsInHeaderDelete(int *recordCount, int *index, FILE *tarFile, int *flag) { 
    if (*index+1 != *recordCount)
        verboseDelete(9, NULL);
    // Si estamos haciendo un delete con fusion, vamos a correr los registros despues de donde estamos actualmente 
    // Direccion: <-
    int numFilesToMove; // Cantidad de registros que se deben correr 
    // Crear un buffer del tamaño de los registros a mover
    struct HeaderRecord *buffer = malloc(sizeof(struct headerRecord) * numFilesToMove);

    size_t elementSize = sizeof(struct headerRecord);
    size_t totalSize = (*recordCount) * elementSize;

    if (*flag == 1) {   // Correr los registros en caso de que el vecino anterior estuviera borrado
        size_t currentPosition = (*index) * elementSize;
        size_t remainingElements = (totalSize - currentPosition) / elementSize;
        numFilesToMove = (int)remainingElements - 1;
       
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index+1), SEEK_SET);  
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);    // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer);
    } else if (*flag == 2) {    // Correr los registros en caso de que el vecino posterior estuviera borrado
        size_t currentPosition = (*index+1) * elementSize;
        size_t remainingElements = (totalSize - currentPosition) / elementSize;
        numFilesToMove = (int)remainingElements - 1;
        
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index+2), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile); // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index+1), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer); 
    } else {
        size_t currentPosition = (*index+1) * elementSize;
        size_t remainingElements = (totalSize - currentPosition) / elementSize;
        numFilesToMove = (int)remainingElements - 1;
        
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index+2), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile); // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer); 
    }
}

/*
Funcion para verificar si los registros contiguos a uno en especifico han sido eliminados 
*/
int checkDeletedNeighbors(FILE *tarFile, struct headerRecord record, int *recordCount, int *index) {
    /*
    Significado de los valores de flag:
    0: No tiene vecinos eliminados
    1: El registro anterior ha sido eliminado
    2: El registro posterior ha sido eliminado
    3: Los registros anterior y posterior han sido eliminados
    */
    verboseDelete(7, NULL);
    int flag = 0;
    struct headerRecord preNeighborRecord, postNeighborRecord;

    // Obtener el registro anterior y posterior, en caso de que existan
    fseek(tarFile,sizeof(int)+(sizeof(record)*(*index-1)),SEEK_SET); 
    fread(&preNeighborRecord,sizeof(record),1,tarFile); 
    fseek(tarFile,sizeof(int)+(sizeof(record)*(*index+1)),SEEK_SET); 
    fread(&postNeighborRecord,sizeof(record),1,tarFile); 

    if (*index == 0) {
        if (postNeighborRecord.deleted == true) {
            flag = 2;
            return flag;
        }
    }
    if (preNeighborRecord.deleted == true && postNeighborRecord.deleted == false) {
        flag = 1;
    } else if (preNeighborRecord.deleted == false && postNeighborRecord.deleted == true) {
        flag = 2;
    } else if (preNeighborRecord.deleted == true && postNeighborRecord.deleted == true) {
        flag = 3;
    } else {
        flag = 0;
        verboseDelete(10, NULL);
    }
    return flag; 
}

/*
Función para eliminar un registro que tiene el vecino anterior marcado como deleted
*/
void deleteCaseOne(FILE *tarFile, struct headerRecord record, int *recordCount, int *index, int *flag) {
        if (*index+1 != *recordCount)
            verboseDelete(8, NULL);

        struct headerRecord preNeighborRecord;
        fseek(tarFile, sizeof(int)+(sizeof(record)*(*index-1)), SEEK_SET);  // Obtener el registro anterior
        fread(&preNeighborRecord,sizeof(preNeighborRecord),1,tarFile); 
        preNeighborRecord.size += record.size;                              // Actualizar tamaño
        fseek(tarFile, sizeof(int)+(sizeof(record)*(*index-1)), SEEK_SET);
        fwrite(&preNeighborRecord, sizeof(preNeighborRecord), 1, tarFile);  // Actualizar header
        // Correr los registros para borrar el registro que quedó fusionado
        moveRegsInHeaderDelete(recordCount, index, tarFile, flag);       
}

/*
Función para eliminar un registro que tiene el vecino posterior marcado como deleted
*/
void deleteCaseTwo(FILE *tarFile, struct headerRecord record, int *recordCount, int *index, int *flag) {
    if (*index+1 != *recordCount)
        verboseDelete(8, NULL);

    struct headerRecord postNeighborRecord;
    fseek(tarFile, sizeof(int)+(sizeof(record)*(*index+1)), SEEK_SET);  // Obtener el registro posterior
    fread(&postNeighborRecord,sizeof(postNeighborRecord),1,tarFile); 
    record.size += postNeighborRecord.size;                             // Actualizar tamaño 
    fseek(tarFile, sizeof(int)+(sizeof(record)*(*index)), SEEK_SET);
    fwrite(&record, sizeof(postNeighborRecord), 1, tarFile);            // Actualizar header
    // Correr los registros para borrar el registro que quedó fusionado
    moveRegsInHeaderDelete(recordCount, index, tarFile, flag);    
}

/*
Función para eliminar un registro que tiene ambos vecinos marcados como deleted
*/
void deleteCaseThree(FILE *tarFile, struct headerRecord record, int *recordCount, int *index, int *flag) { 
    if (*index+1 != *recordCount)
        verboseDelete(8, NULL);

    struct headerRecord preNeighborRecord, postNeighborRecord;
    fseek(tarFile, sizeof(int)+(sizeof(record)*(*index-1)), SEEK_SET);  // Obtener el registro anterior
    fread(&preNeighborRecord,sizeof(preNeighborRecord),1,tarFile); 
    fseek(tarFile, sizeof(int)+(sizeof(record)*(*index+1)), SEEK_SET);  // Obtener el registro posterior
    fread(&postNeighborRecord,sizeof(postNeighborRecord),1,tarFile); 

    preNeighborRecord.size += record.size + postNeighborRecord.size; // Se va a sumar los tamaños al primer registro de los tres
    fseek(tarFile, sizeof(int)+(sizeof(record)*(*index-1)), SEEK_SET);  // Actualizar info en el header
    fwrite(&preNeighborRecord, sizeof(preNeighborRecord), 1, tarFile); 

    // Hacer el corrimiento de los registros, donde ya no se toman en cuenta 
    // el segundo y tercer registro de la tripleta
    moveRegsInHeaderDelete(recordCount, index, tarFile, flag);
}

/*
Funcion para imprimir los verbose del Update
*/
void verboseUpdate(int comment, char *fileName) {
    if (verbose) {
        switch(comment) {
            case 11:
                printf("Intentando abrir el archivo: %s\n", fileName);
                break;
            case 12:
                printf("El archivo: %s no existe, continuando con el siguiente...\n", fileName);
                break;
            case 13:
                printf("Archivo abierto con exito\n");
                break;
            case 14:
                printf("Archivo actualizado con exito\n");
                break;
            default:
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 1:
                printf("Creando un espacio para insertar un registro en la siguiente posicion del header...\n");
                break;
            case 2:
                printf("Revisando si el siguiente registro es un espacio libre lo suficientemente grande...\n");
                break;
            case 3:
                printf("Buscando un espacio libre desde el inicio del header...\n");
                break;
            case 4:
                printf("Espacio libre encontrado\n");
                break;
            case 5:
                printf("Espacio libre NO encontrado\n");
                break;
            case 6:
                printf("Borrando un registro del header...\n");
                break;
            case 7:
                printf("Buscando el espacio libre del final...\n");
                break;
            case 8:
                printf("Buscando si el archivo requiere actualizarse...\n");
                break;
            case 9:
                printf("El archivo requiere actualizarse\n");
                break;
            case 10:
                printf("El archivo NO requiere actualizarse\n");
                break;
            case 15:
                printf("Obteniendo tamaño del archivo...\n");
                break;
            case 16:
                printf("Creando espacio en memoria para cargarlo..\n");
                break;
            case 17:
                printf("Cargando archivo en memoria...\n");
                break;
            case 18:
                printf("El archivo actualizado es del mismo tamaño que el anterior\n");
                break;
            case 19:
                printf("El archivo actualizado es mas grande que el anterior\n");
                break;
            case 20:
                printf("El archivo actualizado es mas pequeño que el anterior\n");
                break;
            case 21:
                printf("Ubicando archivo...\n");
                break;
            case 22:
                printf("Actualizando archivo...\n");
                break;
            case 23:
                printf("Revisando si se puede mantener el archivo en la misma posicion...\n");
                break;
            case 24:
                printf("Hay que mover el archivo...\n");
                break;
            case 25:
                printf("El archivo se puede mantener en la misma posicion\n");
                break;
            case 26:
                printf("El archivo cubre todo el espacio anterior y del espacio vacio contiguo\n");
                break;
            case 27:
                printf("El archivo cubre todo el espacio anterior y una parte del espacio vacio contiguo\n");
                break;
            case 28:
                printf("Actualizando el registro actual del header...\n");
                break;
            case 29:
                printf("Guardando nueva cantidad de registros...\n");
                break;
            case 30:
                printf("Actualizando el registro del espacio vacio contiguo...\n");
                break;
            case 31:
                printf("Marcando registro actual como borrado...\n");
                break;
            case 32:
                printf("Obteniendo registro del espacio disponible...\n");
                break;
            case 33:
                printf("El archivo cubre todo el espacio disponible...\n");
                break;
            case 34:
                printf("Actualizando registro del espacio vacio disponible...\n");
                break;
            case 35:
                printf("El archivo cubre menos espacio del espacio disponible...\n");
                break;
            case 36:
                printf("Agregando nuevo registro con el espacio sobrante disponible...\n");
                break;
            case 37:
                printf("Agregando nuevo registro para el archivo movido al header\n");
                break;
            default:
                break;
        } 
    }
}

// Con esta funcion decimos que despues de un registro en el indice indicado en X
// vamos a liberar ese espacio para insertar un registro
// voviendo todos los registros despues de ese indice en 1
void insertNewRecord(FILE *tarFile, int offset, int recordCount) {
    verboseUpdate(1,NULL);
    char *headerBuffer;
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset+1)),SEEK_SET);
    headerBuffer = (char *)malloc(sizeof(struct headerRecord)*(recordCount-offset-1));
    fread(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset+2)),SEEK_SET);
    fwrite(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    free(headerBuffer);
}

// Esta funcion busca si el siguiente header al actual es un espacio vacio
// al que si le sumamos ese espacio al actual 
bool isNextRecordAvailable(FILE *file, int size, int oldSize, int currentRecord, struct headerRecord *fileHeaderRecord) {
    verboseUpdate(2,NULL);

    fseek(file,sizeof(int)+(sizeof(struct headerRecord)*(currentRecord+1)),SEEK_SET); // nos movemos de registro al siguiente
    fread(fileHeaderRecord,sizeof(struct headerRecord),1,file); // obtenemos el registro

    if((*fileHeaderRecord).size+oldSize >= size && (*fileHeaderRecord).deleted) {
        return true;
    }
    return false;
}

// Esta funcion busca el header que represente un espacio vacio
// y que cumpla con tener un espacio igual o mayor al indicado
// por parametro
int nextFreeSpaceRecordIndex(FILE *file, int size, int recordCount) {
    verboseUpdate(3,NULL);
    struct headerRecord fileHeaderRecord;
    for(int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo
        fseek(file,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // nos movemos de registro al siguiente
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,file); // obtenemos el registro

        if(fileHeaderRecord.size >= size && fileHeaderRecord.deleted) {
            verboseUpdate(4,NULL);
            return i;
        }
    }
    verboseUpdate(5,NULL);
    return -1;
}

// Funcion que borra el registro actual manteniendo los otros
void deleteCurrentRecord(FILE *tarFile, int offset, int recordCount) {
    verboseUpdate(6,NULL);
    char *headerBuffer;
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset+1)),SEEK_SET);
    headerBuffer = (char *)malloc(sizeof(struct headerRecord)*(recordCount-offset-1));
    fread(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset)),SEEK_SET);
    fwrite(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    free(headerBuffer);
}

// Funcion que obtiene el final de los archivos donde podemos
// escribir un archivo que no entre en ningun lado, esto ocurre porque el
// delete no reduce el tamaño del archivo y no se puede usar un SEEK_END para 
// ir ahi
int getRealEndOfFile(FILE *tarFile, int recordCount) {
    verboseUpdate(7,NULL);
    struct headerRecord fileHeaderRecord;
    fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*(recordCount-1)),SEEK_SET);
    fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
    return fileHeaderRecord.start + fileHeaderRecord.size;
}

// Funcion que busca si se puede mantener el mismo registro para cuando el archivo es mas grande
// que antes, revisando si el siguienter record es un espacio vacio 
bool keepUpdatedBiggerFile(
    FILE *tarFile,
    char *buffer,
    struct headerRecord fileHeaderRecord,
    int *currentRecord,
    int *newFileSize,
    int *recordCount,
    int *newRecordCount
) { 
    verboseUpdate(23,NULL);
    if(*currentRecord+1 >= *recordCount) {
        verboseUpdate(24,NULL);
        return false;
    }
    struct headerRecord availableHeaderRecord;
    if(isNextRecordAvailable(tarFile,*newFileSize,fileHeaderRecord.size,*currentRecord,&availableHeaderRecord)) {
        verboseUpdate(25,NULL);

        if(availableHeaderRecord.size + fileHeaderRecord.size == *newFileSize) {
            verboseUpdate(26,NULL);
            // agregamos el espacio vacio al registro actual
            fileHeaderRecord.size += availableHeaderRecord.size;
            
            // Borramos el siguiente record que era el borrado
            deleteCurrentRecord(tarFile,*currentRecord+1,*newRecordCount);

            verboseUpdate(28,NULL);
            // Vamos al registro actual
            fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(*currentRecord)),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(struct headerRecord),1,tarFile); // Escribimos el registro actual

            verboseUpdate(21,NULL);
            // Guardamos el archivo
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            verboseUpdate(22,NULL);
            fwrite(buffer,*newFileSize,1,tarFile);
            
            verboseUpdate(29,NULL);
            // Guardamos la nueva cantidad de registros
            (*newRecordCount)--;
            fseek(tarFile,0,SEEK_SET);
            fwrite(newRecordCount, sizeof(int), 1, tarFile);
            
            // Como borramos un registro ahora estamos un registro menos adelante en el ciclo
            (*recordCount)--;

            return true;
        // En caso de que sobre espacio del registro siguiente del que robamos espacio
        } else {
            verboseUpdate(27,NULL);
            availableHeaderRecord.size -= (*newFileSize - fileHeaderRecord.size); // Le quitamos al siguiente registro lo que ocupamos
            availableHeaderRecord.start = fileHeaderRecord.start + *newFileSize; // Cambiamos donde inicia el registro siguiente
            fileHeaderRecord.size = *newFileSize; // Le agregamos al registro actual lo que le quitamos al registro siguiente
            
            verboseUpdate(28,NULL);
            // Vamos al registro actual
            fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(*currentRecord)),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(struct headerRecord),1,tarFile); // Escribimos el registro actual, esto nos deja
                                                                             // justo donde hay que escribir el siguiente
            verboseUpdate(30,NULL);
            fwrite(&availableHeaderRecord,sizeof(struct headerRecord),1,tarFile); // escribimos el siguiente registro

            verboseUpdate(21,NULL);
            // Guardamos el archivo
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            verboseUpdate(22,NULL);
            fwrite(buffer,*newFileSize,1,tarFile);

            // Saltamos el siguiente registro porque ya sabemos que esta borrado
            (*currentRecord)++;

            return true;
        }
    }
    verboseUpdate(24,NULL);
    return false;
}  

void moveUpdatedBiggerFile(
    FILE *tarFile,
    char *buffer,
    struct headerRecord fileHeaderRecord,
    int *currentRecord,
    int *newFileSize,
    int *recordCount,
    int *newRecordCount
) {
    char fileName[20];
    int availableFreeSpace;

    verboseUpdate(31,NULL);
    // Ponemos que el registro actual esta borrado porque hay que mover
    // el archivo a un lugar con mas espacio
    fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*(*currentRecord)),SEEK_SET);
    strcpy(fileName, fileHeaderRecord.fileName);
    strcpy(fileHeaderRecord.fileName, ""); // borramos el nombre del archivo del registro
    fileHeaderRecord.deleted = true;
    fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

    int flagResult;

    // Revisar si los registros vecinos han sido eliminados 
    flagResult = checkDeletedNeighbors(tarFile, fileHeaderRecord, newRecordCount, currentRecord);

    switch (flagResult) {
        case 1: // Su vecino anterior esta marcado como deleted
            deleteCaseOne(tarFile, fileHeaderRecord, newRecordCount, currentRecord, &flagResult);
            rewind(tarFile); 
            if (((*currentRecord)+1) != (*newRecordCount)) { // Si no es el ultimo registro, solo se elimina 1
                (*newRecordCount)--;
                (*recordCount)--;

            } else {                          // Si es el ultimo registro, se eliminan 2 porque se fusionó
                (*newRecordCount) -= 2;
                (*recordCount) -= 2;
            }
            
            fwrite(newRecordCount, sizeof(int), 1, tarFile);
            break;        
        case 2: // Su vecino posterior esta marcado como deleted
            deleteCaseTwo(tarFile, fileHeaderRecord, newRecordCount, currentRecord, &flagResult);
            rewind(tarFile); 
            if (((*currentRecord)+2) == (*newRecordCount)) { // Si es el ultimo registro, solo se eliminan 2 porque se fusionó
                (*newRecordCount) -= 2;
                (*recordCount) -= 2;
            } else {                         // Si no es el ultimo registro, se elimina 1
                (*newRecordCount)--;
                (*recordCount)--;
            }
            fwrite(newRecordCount, sizeof(int), 1, tarFile);
            break;
        case 3: // Ambos vecinos estan marcados como deleted
            deleteCaseThree(tarFile, fileHeaderRecord, newRecordCount, currentRecord, &flagResult);
            rewind(tarFile); 
            (*newRecordCount) = (*newRecordCount) - 2;
            (*recordCount) = (*recordCount) - 2;
            fwrite(newRecordCount, sizeof(int), 1, tarFile);
            break;
        default:
            break;
    }

    strcpy(fileHeaderRecord.fileName,fileName); // recuperamos el nombre del archivo

    // Buscamos si hay un espacio disponible
    availableFreeSpace = nextFreeSpaceRecordIndex(tarFile, *newFileSize,*recordCount);
    
    // Aqui ocurren los 3 escenarios
    // En caso de que haya un espacio libre donde se pueda encajar el archivo
    // pueden ocurrir dos cosas
    if(availableFreeSpace > -1) {
        verboseUpdate(22,NULL);
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

        // El nuevo espacio disponible es del mismo tamaño que el archivo actualizado, por ende
        // solo hay que actualizar el registro del espacio disponible y guardar el archivo
        // en el lugar indicado por el registro
        if(fileHeaderRecord.size == *newFileSize) {
            verboseUpdate(33,NULL);
            
            verboseUpdate(34,NULL);
            // Actualizamos el record del nuevo espacio encontrado
            fileHeaderRecord.deleted = false;
            strcpy(fileHeaderRecord.fileName,fileName);
            fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

            verboseUpdate(21,NULL);
            // Actualizamos el archivo
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            verboseUpdate(22,NULL);
            fwrite(&buffer,*newFileSize,1,tarFile);

        // En caso de que el espacio disponible es mayor que el archivo actualizado, hay que dividir
        // el registro en dos, uno que contenga el archivo actualizado y otro que indique el espacio
        // vacio restante
        } else {
            verboseUpdate(35,NULL);
            int newFreeSpace = fileHeaderRecord.size - *newFileSize; // calculamos el espacio que va a sobrar
            
            // Hacemos un espacio para colocar el registro con el espacio sobrante
            insertNewRecord(tarFile, availableFreeSpace, *newRecordCount);

            verboseUpdate(34,NULL);
            // Actualizamos el record del nuevo espacio encontrado
            fileHeaderRecord.deleted = false;
            fileHeaderRecord.size = *newFileSize;
            strcpy(fileHeaderRecord.fileName,fileName);
            fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
            
            verboseUpdate(21,NULL);
            // Actualizamos el archivo
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            verboseUpdate(22,NULL);
            fwrite(&buffer,*newFileSize,1,tarFile);
            
            verboseUpdate(36,NULL);
            // Nuevo record con el espacio sobrante
            fileHeaderRecord.deleted = true;
            fileHeaderRecord.start = fileHeaderRecord.start + fileHeaderRecord.size;
            fileHeaderRecord.size = newFreeSpace;
            strcpy(fileHeaderRecord.fileName,"");
            fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*(availableFreeSpace+1)),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
            
            verboseUpdate(29,NULL);
            // Actualizamos la cantidad de registros en el header
            (*newRecordCount)++;
            fseek(tarFile,0,SEEK_SET);
            fwrite(newRecordCount, sizeof(int), 1, tarFile);

            // corregimos el offset al agregar un espacio vacio nuevo
            (*recordCount)++;
        }
    
    // El Ultimo caso seria donde no hay un espacio disponible por lo que hay que guardar el archivo actualizado
    // al final de los archivos
    } else {

        int endOfFilePosition = getRealEndOfFile(tarFile,*newRecordCount);

        verboseUpdate(21,NULL);
        // Agregamos el archivo al final
        fseek(tarFile,endOfFilePosition,SEEK_SET);
        fileHeaderRecord.start = ftell(tarFile);
        verboseUpdate(22,NULL);
        fwrite(buffer,*newFileSize,1,tarFile);

        verboseUpdate(37,NULL);
        // Agregamos el nuevo record al header
        fileHeaderRecord.size = *newFileSize;
        fileHeaderRecord.deleted = false;
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*(*newRecordCount)),SEEK_SET);
        fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
        
        verboseUpdate(29,NULL);
        // Guardamos la nueva cantidad de records
        (*newRecordCount)++;
        fseek(tarFile,0,SEEK_SET);
        fwrite(newRecordCount, sizeof(int), 1, tarFile);
    }
}

void updateStar(int parameterCount, char *parameters[]) {
    char *buffer;
    FILE *currentUpdatedFile;
    int recordCount, newRecordCount, fileNameIndex, newFileSize;
    struct headerRecord fileHeaderRecord;
    
    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    // Abrir el archivo, si no existe se da error.
    // Si no existiera no hay que actualizar
    FILE *tarFile = fopen(tarName, "r+");
    if(tarFile == NULL) {
        printf("El archivo no existe\n");
        exit(1);
    }

    // Lo primero que se escribe en el header es la cantidad de registros que tiene para
    // poder saber si hay archivos, y si hay cuantos son para recorrer todos los registros
    fread(&recordCount,sizeof(int),1,tarFile);
    newRecordCount = recordCount;

    // Si tiene 0 registros significa que no hay nada para actualizar
    if(recordCount <= 0) {
        printf("El star no tiene archivos empaquetados\n");
        fclose(tarFile);
        exit(1);
    }

    // Con este ciclo vamos a pasar por cada uno de los registros del archivo a ver
    // si coincide con uno de los indicados en los parametros del comando
    for(int i = 0 ; i < recordCount; i++) {
        verboseExtract(3,NULL);
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); //nos movemos de registro al siguiente
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile); // obtenemos el registro
        
        // si el registro actual es de un archivo borrado, nos lo saltamos
        if(fileHeaderRecord.deleted) {
            verboseExtract(4,NULL);
            continue;
        }
        
        verboseExtract(5,fileHeaderRecord.fileName);

        verboseUpdate(8,NULL);
        // Buscamos a ver si el nombre del registro actual coincide con uno de la lista de
        // archivos indicado en los parametros del comando
        fileNameIndex = -1;
        for(int i = fileNamesStartIndex ; i < parameterCount ; i++) {
            if(strcmp(fileHeaderRecord.fileName,parameters[i]) == 0) {
                fileNameIndex = i;
                verboseUpdate(9,NULL);
                break;
            }
        }

        // Si es -1 significa que el registro actual no requiere ser actualizado
        if(fileNameIndex == -1) {
            verboseUpdate(10,NULL);
            continue;
        }

        verboseUpdate(11,fileHeaderRecord.fileName);
        // Abrimos el archivo a actualizar
        currentUpdatedFile = fopen(parameters[fileNameIndex], "r");

        if(currentUpdatedFile == NULL) {
            verboseUpdate(12,fileHeaderRecord.fileName);
            continue;
        }

        verboseUpdate(13,fileHeaderRecord.fileName);
        
        verboseUpdate(15,NULL);
        
        // Guardamos el tamaño del nuevo archivo
        fseek(currentUpdatedFile, 0, SEEK_END);
        newFileSize = ftell(currentUpdatedFile);
        rewind(currentUpdatedFile); 
        
        verboseUpdate(16,NULL);
        // Creamos un espacio en memoria del tamaño del archivo a empaquetar
        buffer = (char *)malloc(newFileSize);

        verboseUpdate(17,NULL);
        fread(buffer, newFileSize, 1, currentUpdatedFile); // cargamos el archivo a memoria

        // En caso de que el archivo actualizado sea del mismo tamaño que el empaquetado en el star
        // Solo se sobre escriben los datos a partir de donde empiecen y el header queda igual
        if (newFileSize == fileHeaderRecord.size) {
            verboseUpdate(18,NULL);
            verboseUpdate(21,NULL);
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            verboseUpdate(22,NULL);
            fwrite(buffer,newFileSize,1,tarFile);
            
        // Si el tamaño del nuevo archivo es mas grande que el espacio del archivo empaquetado
        // pueden ocurrir 3 cosas que se explican mas adelante
        } else if(newFileSize >= fileHeaderRecord.size) {
            verboseUpdate(19,NULL);

            if(!keepUpdatedBiggerFile(
                tarFile,
                buffer,
                fileHeaderRecord,
                &i,
                &newFileSize,
                &recordCount,
                &newRecordCount
            )) {
                moveUpdatedBiggerFile(
                    tarFile,
                    buffer,
                    fileHeaderRecord,
                    &i,
                    &newFileSize,
                    &recordCount,
                    &newRecordCount
                );
            }

        // Si el tamaño del nuevo archivo es mas pequeño que el espacio del archivo anterior, entonces
        // hay que agregar un registro que indique el espacio sobrante vacio
        } else {

            verboseUpdate(20,NULL);
            int newFreeSpace;

            // Abrir un espacio siguiente al record actual para colocar un registro con el espacio sobrante
            insertNewRecord(tarFile, i, newRecordCount);
        
            // Guardamos temporalmente el tamaño del nuevo espacio vacio
            newFreeSpace = fileHeaderRecord.size - newFileSize;
            
            // Asignamos el nuevo tamaño al record
            fileHeaderRecord.size = newFileSize;
            
            // Guardamos el archivo actualizado
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            fwrite(buffer,newFileSize,1,tarFile);

            // Guardamos el header del archivo con el espacio actualizado
            fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET);
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

            // Guardamos el header del espacio vacio si no nos encontramo en el
            // ultimo lugar del archivo, si no, no hubiera sentido de dejar un espacio
            // vacio ya que lo que resta es un espacio vacio ilimitado
            
            strcpy(fileHeaderRecord.fileName, "");
            fileHeaderRecord.start = fileHeaderRecord.start + fileHeaderRecord.size;
            fileHeaderRecord.size = newFreeSpace;
            fileHeaderRecord.deleted = true;
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

            // Guardamos la nueva cantidad de records
            newRecordCount++;
            fseek(tarFile,0,SEEK_SET);
            fwrite(&newRecordCount, sizeof(int), 1, tarFile);

            // Ahora el ultimo records se encuentra 1 lugar mas lejos que antes y el siguiente registro
            // ahora es un hueco por lo que lo arreglamos sumando 1 a i y recordCount
            i++;
            recordCount++;
        }

        verboseUpdate(14,NULL);

        strcmp(parameters[i],""); // Eliminamos de la lista de parametros el archivo que acabamos de actualizar

        verboseCreate(10,NULL);
        //liberar recursos
        free(buffer);
        fclose(currentUpdatedFile);
    }

    fclose(tarFile);
}

/*
Funcion para imprimir los verbose del Pack
*/
void verbosePack(int comment, char *fileName) {
    if (verbose) {
        switch(comment) {
            case 2:
                printf("Espacio vacio encontrado\n");
                break;
            case 5:
                printf("Espacio vacio desfragmentado\n");
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 1:
                printf("Obteniendo tamaño final del star...\n");
                break;
            case 3:
                printf("Espacio ocupado encontrado, ignorando...\n");
                break;
            case 4:
                printf("Truncando el archivo al tamaño real...\n");
                break;
            case 6:
                printf("Moviendo todos los archivos y registros hacia atras...\n");
                break;
            case 7:
                printf("Moviendo archivo hacia atras: %s\n", fileName);
                break;
            case 8:
                printf("Espacio vacio encontrado, moviendo unicamente el registro del header...\n");
                break;
            case 9:
                printf("Moviendo el inicio del header...\n");
                break;
            default:
                break;
        } 
    }
}

// Funcion que borra un registro, mueve los demas registros para que esten parejos
// y mueve todos los demas archivos hacia atras lo suficiente para rellenar
// el espacio vacio
void defragCurrentSpace(
    FILE *tarFile,
    int *recordCount,
    int *currentRecord,
    struct headerRecord deletedRecord
) {
    char *buffer;
    struct headerRecord currentMovingFile;

    // Borramos el registro actual, moviendo el resto hacia atras
    deleteCurrentRecord(tarFile, *currentRecord,*recordCount);
    (*recordCount)--; // actualizamos la cantidad de registros, ahora hay 1 menos

    verbosePack(6,NULL);
    for(int i = *currentRecord ; i < *recordCount; i++) {
        verboseExtract(3,NULL);
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(currentMovingFile)*i),SEEK_SET); // vamos al siguiente registro
        fread(&currentMovingFile,sizeof(currentMovingFile),1,tarFile); // obtenemos el registro
        
        // Si el registro actual es de un archivo borrado, no hacemos el proceso 
        // de moverlo porque significa que aqui no hay informacion que mover.
        if(!currentMovingFile.deleted) {
            verbosePack(7,currentMovingFile.fileName);
            // Cargar el archivo
            buffer = (char *)malloc(currentMovingFile.size); // hacemos un espacio donde podamos cargar el archivo a memoria
            fseek(tarFile,currentMovingFile.start,SEEK_SET); // buscamos el archivo
            fread(buffer,currentMovingFile.size,1,tarFile);  // lo cargamos a memoria

            // Escribir archivo mas atras
            fseek(tarFile,currentMovingFile.start-deletedRecord.size,SEEK_SET); //vamos al nuevo lugar del archivo
            fwrite(buffer,currentMovingFile.size,1,tarFile); // escribimos el archivo

            free(buffer); // Liberar memoria 
        } else {
            verbosePack(8,NULL);
        }

        verbosePack(9,NULL);
        // Guardamos el header del archivo con el inicio actualizado
        currentMovingFile.start -= deletedRecord.size;
        fseek(tarFile,sizeof(int)+(sizeof(currentMovingFile)*i),SEEK_SET);
        fwrite(&currentMovingFile,sizeof(currentMovingFile),1,tarFile);
    }
    (*currentRecord)--; // En caso que el siguiente registro sea un hueco vacio, este ahora esta en
                        // la posicion actual y no se va a revisar, entonces se resta en 1 para que
                        // el siguiente ciclo donde se ejecuto esta funcion, logre ver este registro
                        // sin embargo esto no deberia ser posible porque ya debeian estar fusionados
                        // pero se coloca de todas formas por si acaso.
}

void packStar(int parameterCount, char *parameters[]) {
    char *buffer;
    int recordCount, tarFinalSize;
    FILE *currentExtractFile;
    struct headerRecord fileHeaderRecord;

    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    // Abrir el archivo, si no existe se da error.
    // Si no existiera no hay que extraer
    FILE *tarFile = fopen(tarName, "r+");
    if(tarFile == NULL) {
        printf("El archivo no existe\n");
        exit(1);
    }

    // Lo primero que se escribe en el header es la cantidad de registros que tiene para
    // poder saber si hay archivos, y si hay cuantos son para recorrer todos los registros
    fread(&recordCount,sizeof(int),1,tarFile);

    // Si tiene 0 registros significa que no hay nada para desfragmentar
    if(recordCount <= 0) {
        printf("El star no tiene archivos empaquetados\n");
        fclose(tarFile);
        exit(1);
    }

    // Con este ciclo vamos extrayendo uno por uno los archivos que hay segun el header
    for(int i = 0 ; i < recordCount; i++) {
        verboseExtract(3,NULL);
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // vamos al primer registro
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile); // obtenemos el registro
        
        if(i == recordCount-1) {
            verbosePack(1,NULL);
            tarFinalSize = fileHeaderRecord.start + fileHeaderRecord.size;
        }

        // si el registro actual es de un archivo existente, nos lo saltamos
        if(!fileHeaderRecord.deleted) {
            verbosePack(3,NULL);
            continue;
        }
        verbosePack(2,NULL);

        // En caso de encontrar un espacio vacio, se borra su registro
        // y se mueven todos los otros registros 1 hacia atras
        // y los archivos se mueven hacia atras la cantidad de espacio
        // que existe en ese lugar
        defragCurrentSpace(
            tarFile,
            &recordCount,
            &i,
            fileHeaderRecord
        );
        verbosePack(5,NULL);
    }

    verboseUpdate(29,NULL);
    // Guardamos la nueva cantidad de records
    fseek(tarFile,0,SEEK_SET);
    fwrite(&recordCount, sizeof(int), 1, tarFile);

    verbosePack(4,NULL);
    truncate(tarName,tarFinalSize); // se pide que el tamaño del archivo represente el tamaño real
    fclose(tarFile);
}

/*
Funcion para imprimir los verbose simples segun sea el caso 
*/
void verboseAppend(int comment, char *fileName[], struct headerRecord newRecord) {
    if (verbose || vverbose) {
        switch(comment) {
            case 1:
                printf("Intentando abrir el archivo: %s\n", fileName[0]); 
                break;
            case 2:
                printf("Se logró abrir el archivo: %s\n", fileName[0]); 
                break;
            case 3:
                printf("Se empaquetó el archivo: %s\n", newRecord.fileName);
                break;
            default:
                break;
        }
    } 
    if (vverbose) {
        switch(comment) {
            case 4:
                printf("El star estaba vacío, se empaquetó el archivo %s\n", fileName[0]); 
                break;
            case 5:
                printf("Se encontró un espacio libre\n"); 
                break;
            case 6:
                printf("No sobró espacio disponible en el espacio\n");
                break;
            case 7:
                printf("Sobró espacio disponible en el espacio\n");
                break;
            case 8:
                printf("Se movieron los registros para añadir al nuevo registro\n");
                break;
            case 9:
                printf("No hay espacios para reutilizar, se agregó al final\n");
                break;
            default:
                break;
        } 
    }
}

/*
Funcion que solo agrega un registro y empaqueta archivo nuevo
*/
void appendFile(FILE *tarFile, struct headerRecord newRecord, int *index,
                const char *filename) {
    char *buffer;

    // Buscamos la posicion donde debemos guardar el nuevo registro en el header
    fseek(tarFile, (sizeof(newRecord)*(*index))+sizeof(int), SEEK_SET);
    fwrite(&newRecord,sizeof(newRecord),1,tarFile); // Guardar el registro en el header
    
    // Creamos un espacio en memoria del tamaño del archivo a empaquetar
    buffer = (char *)malloc(newRecord.size);
    FILE *fileToAdd = fopen(filename, "r");
    fread(buffer, newRecord.size, 1, fileToAdd); // Cargar el archivo a memoria
    fseek(tarFile, newRecord.start, SEEK_SET);   // Posicionarse donde se va a guardar el archivo
    fwrite(buffer,newRecord.size,1,tarFile);     // Escribir archivo
    free(buffer);   // liberamos memoria
    fclose(fileToAdd);
}

/*
Funcion encargada de realizar el corrimiento de los registros cuando se realiza un append
*/
void moveRegsInHeaderAppend(int *recordCount, int *index, FILE *tarFile, int flag) {
    int numFilesToMove; // Cantidad de registros que se deben correr 
    struct HeaderRecord *buffer = malloc(sizeof(struct headerRecord) * numFilesToMove);
    
    // Leer los registros despues del que se va a eliminar/agregar 
    // Si se esta haciendo un append, vamos a correr los registros a partir de donde estamos actualmente
    // Direccion: ->
    size_t elementSize = sizeof(struct headerRecord);
    size_t totalSize = (*recordCount) * elementSize;
    size_t currentPosition = (*index) * elementSize;
    size_t remainingElements = (totalSize - currentPosition) / elementSize;

    if (flag == 1) {
        numFilesToMove = (int)remainingElements;    // No quedan registros con size 0 
        // Crear un buffer del tamaño de los registros a mover
        struct HeaderRecord *buffer = malloc(sizeof(struct headerRecord) * numFilesToMove);
        
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index+1), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
    } else {
        numFilesToMove = (int)remainingElements - 1;    // Caso de que quede algun registro con size 0
        // Crear un buffer del tamaño de los registros a mover
        struct HeaderRecord *buffer = malloc(sizeof(struct headerRecord) * numFilesToMove);
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(int) + sizeof(struct headerRecord)*(*index), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
    }
    free(buffer);// Liberar el buffer
}

/*
Funcion encargada de hacer un append cuando se encuentra un registro que fue borrado 
y posee espacio suficiente para insertar el nuevo registro y empaquetar el archivo
*/
void firstCaseAppend(FILE *tarFile, struct headerRecord newRecord, struct headerRecord readHeaderRecord, 
                int *recordCount, const char *filename, int *index) {
    // Se actualiza el tamaño del registro para demostrar el espacio disponible
    newRecord.start = readHeaderRecord.start;
    readHeaderRecord.size = (readHeaderRecord.size - newRecord.size);   

    if (readHeaderRecord.size == 0) {
        verboseAppend(6, NULL, newRecord);
        fseek(tarFile, (sizeof(readHeaderRecord)*(*index))+sizeof(int), SEEK_SET); //Actualizar info del registro en deleted
        fwrite(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile);
        moveRegsInHeaderAppend(recordCount, index, tarFile, 2);
        // Agregar el nuevo registro y empaquetar archivo con appendFile
        appendFile(tarFile, newRecord, index, filename);
        *recordCount = *recordCount - 1;
        verboseAppend(8, NULL, newRecord);
    } else {
        verboseAppend(7, NULL, newRecord);
        readHeaderRecord.start = readHeaderRecord.start + newRecord.size;   // Posicion donde termina el nuevo archivo
        fseek(tarFile, (sizeof(readHeaderRecord)*(*index))+sizeof(int), SEEK_SET); //Actualizar info del registro en deleted
        fwrite(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile);
        moveRegsInHeaderAppend(recordCount, index, tarFile, 1);
        // Agregar el nuevo registro y empaquetar archivo con appendFile
        appendFile(tarFile, newRecord, index, filename); 
        verboseAppend(8, NULL, newRecord);
    }
}

/*
Funcion para manejar el proceso de agregado de un archivo
*/
void appendStar(int parameterCount, char *parameters[]) {
    char *buffer;
    int recordCount = 0;
    int lastFilePosition = HEADEREND;
    FILE *fileToAdd;

    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    FILE *tarFile = fopen(tarName, "r+");
    struct headerRecord newRecord;
    bool addedFile = false;

    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        exit(1);
    } 
    fread(&recordCount,sizeof(int),1,tarFile);  // Ver cuantos registros hay actualmente

    for (int i = fileNamesStartIndex ; i < parameterCount ; i++) {
        // Si el nombre del archivo es superior a 20 caracteres no podemos empaquetar ese archivo y pasamos al siguiente
        if (strlen(parameters[i]) > 20) {
            printf("El archivo: %s tiene un nombre mayor a 20 caracteres\n", parameters[i]);
            continue;
        }
        verboseAppend(1, &parameters[i], newRecord);

        // Verificar que el archivo que se desea agregar exista
        fileToAdd = fopen(parameters[i], "r"); 
        if(fileToAdd == NULL) { // Si no existe pasamos al siguiente
            printf("El archivo: %s no existe, continuando con el siguiente parametro si hay\n", parameters[i]);
            continue;
        }
        verboseAppend(2, &parameters[i], newRecord);

        if (checkDuplicate(tarFile, &parameters[i], &recordCount)){
            printf("Quisiste decir -u/--update?\n");
            exit(1);
        }
        
        strcpy(newRecord.fileName,parameters[i]); // Asignar nombre del archivo
        fseek(fileToAdd, 0, SEEK_END); // Buscar tamaño del archivo
        newRecord.size = ftell(fileToAdd);
        newRecord.deleted = false;

        // Caso de que no se hayan agregado archivos
        if (recordCount == 0) {
            verboseAppend(4, &parameters[i], newRecord);
            newRecord.start = lastFilePosition;
            appendFile(tarFile, newRecord, &recordCount, parameters[i]);
            recordCount++;
            continue;
        }

        // Con este ciclo vamos recorriendo para la tecnica de primer ajuste
        for (int rec = 0 ; rec < recordCount; rec++) {
            //Obtener el header del archivo actual
            struct headerRecord readHeaderRecord;
            fseek(tarFile,sizeof(int)+(sizeof(readHeaderRecord)*rec),SEEK_SET); // vamos al registro
            fread(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile); // obtenemos el registro
            
            // Caso 1: Si el registro actual es de un archivo borrado, verificamos si hay espacio para el archivo nuevo
            if (readHeaderRecord.deleted) {
                if (newRecord.size <= readHeaderRecord.size) {
                    verboseAppend(5, &parameters[i], newRecord);
                    firstCaseAppend(tarFile, newRecord, readHeaderRecord, &recordCount, parameters[i], &rec);
                    recordCount++;
                    addedFile = true;
                }
                break;
            }
        }
        // Caso 2: No se encontró espacio entre archivos entonces se agrega despues del ultimo archivo agregado  
        if (addedFile == false) {
            struct headerRecord readHeaderRecord;
            fseek(tarFile,sizeof(int)+(sizeof(readHeaderRecord)*(recordCount-1)),SEEK_SET); // vamos al registro
            fread(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile); // obtenemos el registro
            newRecord.start = readHeaderRecord.start + readHeaderRecord.size;
            appendFile(tarFile, newRecord, &recordCount, parameters[i]);
            fclose(fileToAdd);       
            recordCount++;
            verboseAppend(9, &parameters[i], newRecord);
        }
    }
    // Guardamos la cantidad de registros al inicio del header
    rewind(tarFile); 
    fwrite(&recordCount, sizeof(int), 1, tarFile);
    fclose(tarFile);
    verboseAppend(3, &parameters[0], newRecord);
}

/*
Funcion para determinar si todos los registros del header fueron eliminados logicamente
*/
bool checkEmptyStar(FILE *tarFile, int *recordCount) {
    bool emptyStar = true;
    for (int i = 0 ; i < *recordCount; i++) {
        struct headerRecord record;
        fseek(tarFile,sizeof(int)+(sizeof(record)*i),SEEK_SET); // Obtener el header del archivo actual
        fread(&record,sizeof(record),1,tarFile); // obtenemos el registro
        if (record.deleted == false) {
            emptyStar = false;
        }
    }
    return emptyStar;
}

/*
Función encargada de manejar el proceso de borrado de un archivo
*/
void deleteStar(int parameterCount, char *parameters[]) {
    int recordCount = 0, flagResult = 0, numFilesToMove = 0, iter = 0;
    int lastFilePosition = HEADEREND;
    FILE *fileToDelete;

    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    FILE *tarFile = fopen(tarName, "r+");
    struct headerRecord record, preNeighborRecord, postNeighborRecord;
    
    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        exit(1);
    } 
    fread(&recordCount,sizeof(int),1,tarFile); // Ver cuantos registros hay actualmente
    
    // Si tiene 0 registros significa que no hay nada para eliminar
    if(checkEmptyStar(tarFile, &recordCount)) {
        printf("El star no tiene archivos empaquetados\n");
        fclose(tarFile);
        exit(1);
    }
    
    for (int i = fileNamesStartIndex; i < parameterCount ; i++) {
        struct headerRecord recToDelete;

        strcpy(recToDelete.fileName,parameters[i]);
        // Verificar si el archivo se encuentra empaquetado o no
        if (checkDuplicate(tarFile, &parameters[i], &recordCount) == false) {
            printf("El archivo: %s no se encuentra empaquetado, continuando con el siguiente\n", parameters[i]);
            exit(1);
        }

        // Recorrer los registros del header
        for (int index = 0 ; index < recordCount; index++) {
            //Obtener el header del archivo actual
            fseek(tarFile,sizeof(int)+(sizeof(record)*index),SEEK_SET); 
            fread(&record,sizeof(record),1,tarFile); 

            // Cuando encuentre el registro que se desea eliminar
            if (strcmp(record.fileName, parameters[i]) == 0) {
                // Caso 0: No tiene vecinos que esten eliminados
                record.deleted = true;
                strcpy(record.fileName,"");                               // Asignar string vacio para evitar choques en un futuro
                fseek(tarFile, -sizeof(struct headerRecord), SEEK_CUR);   // Mueve el puntero de archivo al inicio del registro actual
                fwrite(&record, sizeof(struct headerRecord), 1, tarFile); // Escribe el registro actualizado en el archivo
            
                // Revisar si los registros vecinos han sido eliminados 
                flagResult = checkDeletedNeighbors(tarFile, record, &recordCount, &index);

                switch (flagResult) {
                    case 0: // Caso especial 0: No tiene vecinos que esten eliminados y es el ultimo registor del header
                        if ((index+1) == recordCount) {
                            recordCount--;
                            rewind(tarFile); 
                            fwrite(&recordCount, sizeof(int), 1, tarFile);
                        }
                        break;
                    case 1: // Su vecino anterior esta marcado como deleted
                        deleteCaseOne(tarFile, record, &recordCount, &index, &flagResult);
                        rewind(tarFile); 
                        if ((index+1) != recordCount) { // Si no es el ultimo registro, solo se elimina 1
                            recordCount--;
                            verboseDelete(4, &parameters[i]);
                        } else                           // Si es el ultimo registro, se eliminan 2 porque se fusionó
                            recordCount -= 2;
                        fwrite(&recordCount, sizeof(int), 1, tarFile);
                        break;        
                    case 2: // Su vecino posterior esta marcado como deleted
                        deleteCaseTwo(tarFile, record, &recordCount, &index, &flagResult);
                        rewind(tarFile); 
                        if ((index+2) == recordCount) { // Si es el ultimo registro, solo se eliminan 2 porque se fusionó
                            recordCount -= 2;
                            verboseDelete(5, &parameters[i]);
                        } else                           // Si no es el ultimo registro, se elimina 1
                            recordCount--;
                        fwrite(&recordCount, sizeof(int), 1, tarFile);
                        break;
                    case 3: // Ambos vecinos estan marcados como deleted
                        verboseDelete(6, &parameters[i]);
                        deleteCaseThree(tarFile, record, &recordCount, &index, &flagResult);
                        rewind(tarFile); 
                        recordCount = recordCount - 2;
                        fwrite(&recordCount, sizeof(int), 1, tarFile);
                        break;
                    default:
                        break;
                }
                verboseDelete(3, &parameters[i]);
            }
        }
    }
    fclose(tarFile);
}

void listStar(int parameterCount, char *parameters[]) {
    int recordCount = 0;
    int lastFilePosition = HEADEREND;
    struct headerRecord record;
    FILE *fileToAdd;

    // -f indica el nombre del archivo segun la documentacion de tar
    if(!file) {
        printf("Falta el parametro -f o --file\n");
        exit(1);
    }

    FILE *tarFile = fopen(tarName, "r");
    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        exit(1);
    } 
    tarFile = fopen(tarName, "r");

    // Obtener cantidad de registros
    fread(&recordCount,sizeof(int),1,tarFile);

    // Si tiene 0 registros significa que no hay nada para listar
    if(checkEmptyStar(tarFile, &recordCount)) {
        printf("El star no tiene archivos empaquetados\n");
        fclose(tarFile);
        exit(1);
    }

    if (verbose) {
        printf("Listando archivos...\n");
    }
    
    for (int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo actual
        fseek(tarFile,sizeof(int)+(sizeof(record)*i),SEEK_SET); // vamos al registro
        fread(&record,sizeof(record),1,tarFile); // obtenemos el registro
      
        if (verbose == true && vverbose == false) {    
            if (record.deleted == false) {          
                printf("FileName: %s, Start: %ld, Size: %ld\n", 
                    record.fileName, record.start, record.size);
            }
        } 
        else if (vverbose) {
            if (record.deleted) {
                printf("~Espacio disponible, Start: %ld, Size: %ld\n", 
                record.start, record.size);
            } else {
                printf("FileName: %s, Start: %ld, Size: %ld, Deleted: %s\n", 
                record.fileName, record.start, record.size, record.deleted ? "true" : "false");
            }
        } else {
            printf("FileName: %s\n", record.fileName);
        }
    }
}


int main(int argc, char *argv[]) {
    parameterChecker(argc,argv);

    if (create) {
        createStar(argc, argv);
    }

    else if (append) {
        appendStar(argc, argv);
    }

    else if (list) {
        listStar(argc, argv);
    }

    else if (extract) {
        extractStar(argc, argv);
    }

    else if (delete) {
        deleteStar(argc, argv);
    }
    
    else if(update) {
        updateStar(argc,argv);
    } 
    
    else if(pack) {
        packStar(argc,argv);
    }

    exit(0);
}