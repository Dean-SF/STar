#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HEADEREND 40*101

struct headerRecord {
    char fileName[20];
    long start;
    long size;
    bool deleted;
};

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
        
        fseek(tarFile, sizeof(struct headerRecord)*(*index), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(struct headerRecord)*(*index+1), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
    } else {
        numFilesToMove = (int)remainingElements - 1;    // Caso de que quede algun registro con size 0
        // Crear un buffer del tamaño de los registros a mover
        struct HeaderRecord *buffer = malloc(sizeof(struct headerRecord) * numFilesToMove);
        fseek(tarFile, sizeof(struct headerRecord)*(*index), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(struct headerRecord)*(*index), SEEK_SET);
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
    char* emptyString = (char*)malloc(1);  // string vacio para el verbose
    // Se actualiza el tamaño del registro para demostrar el espacio disponible
    newRecord.start = readHeaderRecord.start;
    readHeaderRecord.size = (readHeaderRecord.size - newRecord.size);   

    if (readHeaderRecord.size == 0) {
        verboseAppend(6, &emptyString, newRecord);
        fseek(tarFile, (sizeof(readHeaderRecord)*(*index))+sizeof(int), SEEK_SET); //Actualizar info del registro en deleted
        fwrite(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile);
        moveRegsInHeaderAppend(recordCount, index, tarFile, 2);
        // Agregar el nuevo registro y empaquetar archivo con appendFile
        appendFile(tarFile, newRecord, index, filename);
        *recordCount = *recordCount - 1;
        verboseAppend(8, &emptyString, newRecord);
    } else {
        verboseAppend(7, &emptyString, newRecord);
        readHeaderRecord.start = readHeaderRecord.start + newRecord.size;   // Posicion donde termina el nuevo archivo
        fseek(tarFile, (sizeof(readHeaderRecord)*(*index))+sizeof(int), SEEK_SET); //Actualizar info del registro en deleted
        fwrite(&readHeaderRecord,sizeof(readHeaderRecord),1,tarFile);
        moveRegsInHeaderAppend(recordCount, index, tarFile, 1);
        // Agregar el nuevo registro y empaquetar archivo con appendFile
        appendFile(tarFile, newRecord, index, filename); 
        verboseAppend(8, &emptyString, newRecord);
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
    FILE *tarFile = fopen(tarName, "r+");
    struct headerRecord newRecord;
    bool addedFile = false;
    
    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        fclose(tarFile);
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
    char* emptyString = (char*)malloc(1);  
    verboseDelete(7, &emptyString);
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
        verboseDelete(10, &emptyString);
    }
    return flag; 
}

/*
Funcion encargada de realizar el corrimiento de los registros cuando se realiza un delete
*/
void moveRegsInHeaderDelete(int *recordCount, int *index, FILE *tarFile, int *flag) {
    char* emptyString = (char*)malloc(1);  
    if (*index+1 != *recordCount)
        verboseDelete(9, &emptyString);
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
       
        fseek(tarFile, sizeof(struct headerRecord)*(*index+1), SEEK_SET);  
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);    // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(struct headerRecord)*(*index), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer);
    } else if (*flag == 2) {    // Correr los registros en caso de que el vecino posterior estuviera borrado
        size_t currentPosition = (*index+1) * elementSize;
        size_t remainingElements = (totalSize - currentPosition) / elementSize;
        numFilesToMove = (int)remainingElements - 1;
        
        fseek(tarFile, sizeof(struct headerRecord)*(*index+2), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile); // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(struct headerRecord)*(*index+1), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer); 
    } else {
        size_t currentPosition = (*index+1) * elementSize;
        size_t remainingElements = (totalSize - currentPosition) / elementSize;
        numFilesToMove = (int)remainingElements - 1;
        
        fseek(tarFile, sizeof(struct headerRecord)*(*index+2), SEEK_SET);
        fread(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile); // Leer los registros despues del registro borrado
        // Escribir el buffer de vuelta en el archivo desde la posición que se desea
        fseek(tarFile, sizeof(struct headerRecord)*(*index), SEEK_SET);
        fwrite(buffer, sizeof(struct headerRecord), numFilesToMove, tarFile);
        free(buffer); 
    }
}

/*
Función para eliminar un registro que tiene el vecino anterior marcado como deleted
*/
void deleteCaseOne(FILE *tarFile, struct headerRecord record, int *recordCount, int *index, int *flag) {
        char* emptyString = (char*)malloc(1);  
        if (*index+1 != *recordCount)
            verboseDelete(8, &emptyString);

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
    char* emptyString = (char*)malloc(1);  
    if (*index+1 != *recordCount)
        verboseDelete(8, &emptyString);

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
    char* emptyString = (char*)malloc(1);  
    if (*index+1 != *recordCount)
        verboseDelete(8, &emptyString);

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
Función encargada de manejar el proceso de borrado de un archivo
*/
void deleteStar(int parameterCount, char *parameters[]) {
    int recordCount = 0, flagResult = 0, numFilesToMove = 0, iter = 0;
    int lastFilePosition = HEADEREND;
    FILE *fileToDelete;
    FILE *tarFile = fopen(tarName, "r+");
    struct headerRecord record, preNeighborRecord, postNeighborRecord;
    
    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        fclose(tarFile);
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
        verboseDelete(1, &parameters[i]);

        // Verificar que el archivo que se desea eliminar exista
        fileToDelete = fopen(parameters[i], "r"); 
        
        // Si no existe pasamos al siguiente
        if(fileToDelete == NULL) {
            printf("El archivo: %s no existe, continuando con el siguiente si hay\n", parameters[i]);
            continue;
        }
        verboseDelete(2, &parameters[i]);

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

void listStar(int parameterCount, char *parameters[]) {
    int recordCount = 0;
    int lastFilePosition = HEADEREND;
    struct headerRecord record;
    FILE *fileToAdd;
    FILE *tarFile = fopen(tarName, "r");
    if (tarFile == NULL) {
        printf("El archivo no existe\n");
        fclose(tarFile);
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
        if (vverbose) {
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

    if (create) {
        createStar(argc, argv);
    }

    if (append) {
        appendStar(argc, argv);
    }

    if (list) {
        listStar(argc, argv);
    }

    if (extract) {
        extractStar(argc, argv);
    }

    if (delete) {
        deleteStar(argc, argv);
    }

    exit(0);
}