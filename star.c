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

    // En un futuro deberiamos poder recibir archivos desde stdin, por ahora
    // ocupamos el parametro -f para que funcione
    if(!file) {
        printf("stdin not implemented yet");
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

// FOR DEBUGGING
void printHeader(FILE *file) {
    int recordCount;
    struct headerRecord fileHeaderRecord;
    fseek(file,0,SEEK_SET);
    fread(&recordCount,sizeof(int),1,file);
    
    printf("Records: %i\n", recordCount);

    for(int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo
        fseek(file,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // vamos al primer registro
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,file); // obtenemos el registro
        printf("%s %li %li %s\n", fileHeaderRecord.fileName, fileHeaderRecord.start, fileHeaderRecord.size, fileHeaderRecord.deleted ? "true" : "false");
    }
}

// Con esta funcion decimos que despues de un registro en el indice indicado en X
// vamos a liberar ese espacio para insertar un registro
// voviendo todos los registros despues de ese indice en 1
void insertNewRecord(FILE *tarFile, int offset, int recordCount) {
    char *headerBuffer;
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset+1)),SEEK_SET);
    headerBuffer = (char *)malloc(sizeof(struct headerRecord)*(recordCount-offset-1));
    fread(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    fseek(tarFile,sizeof(int)+(sizeof(struct headerRecord)*(offset+2)),SEEK_SET);
    fwrite(headerBuffer,sizeof(struct headerRecord)*(recordCount-offset-1),1,tarFile);
    free(headerBuffer);
}

// Esta funcion busca el header que represente un espacio vacio
// y que cumpla con tener un espacio igual o mayor al indicado
// por parametro
int nextFreeSpaceRecordIndex(FILE *file, int size, int recordCount) {
    struct headerRecord fileHeaderRecord;
    for(int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo
        fseek(file,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); // nos movemos de registro al siguiente
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,file); // obtenemos el registro

        if(fileHeaderRecord.size >= size && fileHeaderRecord.deleted) {
            return i;
        }
    }
    return -1;
}

void updateStar(int parameterCount, char *parameters[]) {
    char *buffer;
    FILE *currentUpdatedFile;
    int recordCount, newRecordCount, fileNameIndex, newFileSize;
    struct headerRecord fileHeaderRecord;

    // Abrir el archivo, si no existe se da error.
    // Si no existiera no hay que actualizar
    FILE *tarFile = fopen(tarName, "r+");
    if(tarFile == NULL) {
        printf("El archivo no existe\n");
        fclose(tarFile);
        exit(1);
    }

    // En un futuro deberiamos poder recibir archivos desde stdin, por ahora
    // ocupamos el parametro -f para que funcione
    if(!file) {
        printf("stdin not implemented yet");
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
    
    // FOR DEBUGGING
    printHeader(tarFile);
    printf("\n");

    // Con este ciclo vamos a pasar por cada uno de los registros del archivo a ver
    // si coincide con uno de los indicados en los parametros del comando
    for(int i = 0 ; i < recordCount; i++) {
        //Obtener el header del archivo
        fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET); //nos movemos de registro al siguiente
        fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile); // obtenemos el registro
        
        // si el registro actual es de un archivo borrado, nos lo saltamos
        if(fileHeaderRecord.deleted) {
            continue;
        }
        
        // Buscamos a ver si el nombre del registro actual coincide con uno de la lista de
        // archivos indicado en los parametros del comando
        fileNameIndex = -1;
        for(int i = fileNamesStartIndex ; i < parameterCount ; i++) {
            if(strcmp(fileHeaderRecord.fileName,parameters[i]) == 0) {
                fileNameIndex = i;
                break;
            }
        }

        // Si es -1 significa que el registro actual no requiere ser actualizado
        if(fileNameIndex == -1) {
            continue;
        }

        // Abrimos el archivo a actualizar
        currentUpdatedFile = fopen(parameters[fileNameIndex], "r");

        // Guardamos el tamaño del nuevo archivo
        fseek(currentUpdatedFile, 0, SEEK_END);
        newFileSize = ftell(currentUpdatedFile);
        rewind(currentUpdatedFile); 
        
        // Creamos un espacio en memoria del tamaño del archivo a empaquetar
        buffer = (char *)malloc(newFileSize);
        fread(buffer, newFileSize, 1, currentUpdatedFile); // cargamos el archivo a memoria

        // En caso de que el archivo actualizado sea del mismo tamaño que el empaquetado en el star
        // Solo se sobre escriben los datos a partir de donde empiecen y el header queda igual
        if (newFileSize == fileHeaderRecord.size) {
            fseek(tarFile,fileHeaderRecord.start,SEEK_SET);
            fwrite(buffer,newFileSize,1,tarFile);
            
        // Si el tamaño del nuevo archivo es mas grande que el espacio del archivo empaquetado
        // pueden ocurrir 3 cosas que se explican mas adelante
        } else if(newFileSize >= fileHeaderRecord.size) {
            char fileName[20];
            int availableFreeSpace;

            // Ponemos que el registro actual esta borrado porque hay que mover
            // el archivo a un lugar con mas espacio
            fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*i),SEEK_SET);
            strcpy(fileName, fileHeaderRecord.fileName);
            strcpy(fileHeaderRecord.fileName, ""); // borramos el nombre del archivo del registro
            fileHeaderRecord.deleted = true;
            fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

            strcpy(fileHeaderRecord.fileName,fileName); // recuperamos el nombre del archivo

            // Buscamos si hay un espacio disponible
            availableFreeSpace = nextFreeSpaceRecordIndex(tarFile, newFileSize,recordCount);
            
            // Aqui ocurren los 3 escenarios
            // En caso de que haya un espacio libre donde se pueda encajar el archivo
            // pueden ocurrir dos cosas
            if(availableFreeSpace > -1) {
                printf("hay espacio disponible\n");
                fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
                fread(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

                // El nuevo espacio disponible es del mismo tamaño que el archivo actualizado, por ende
                // solo hay que actualizar el registro del espacio disponible y guardar el archivo
                // en el lugar indicado por el registro
                if(fileHeaderRecord.size == newFileSize) {
                    printf("del mismo tamaño\n");
                    // Actualizamos el record del nuevo espacio encontrado
                    fileHeaderRecord.deleted = false;
                    strcpy(fileHeaderRecord.fileName,fileName);
                    fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
                    fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

                    // Actualizamos el archivo
                    fseek(tarFile,fileHeaderRecord.start,SEEK_END);
                    fwrite(&buffer,newFileSize,1,tarFile);

                // En caso de que el espacio disponible es mayor que el archivo actualizado, hay que dividir
                // el registro en dos, uno que contenga el archivo actualizado y otro que indique el espacio
                // vacio restante
                } else {
                    printf("de tamaño superior\n");
                    int newFreeSpace = fileHeaderRecord.size - newFileSize; // calculamos el espacio que va a sobrar
                    printf("%li %i\n",fileHeaderRecord.size, newFileSize);
                    
                    // Hacemos un espacio para colocar el registro con el espacio sobrante
                    insertNewRecord(tarFile, availableFreeSpace, newRecordCount);

                    // Actualizamos el record del nuevo espacio encontrado
                    fileHeaderRecord.deleted = false;
                    fileHeaderRecord.size = newFileSize;
                    strcpy(fileHeaderRecord.fileName,fileName);
                    fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*availableFreeSpace),SEEK_SET);
                    fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);

                    // Actualizamos el archivo
                    fseek(tarFile,fileHeaderRecord.start,SEEK_END);
                    fwrite(&buffer,newFileSize,1,tarFile);

                    // Nuevo record con el espacio sobrante
                    fileHeaderRecord.deleted = true;
                    fileHeaderRecord.start = fileHeaderRecord.start + fileHeaderRecord.size;
                    fileHeaderRecord.size = newFreeSpace;
                    strcpy(fileHeaderRecord.fileName,"");
                    fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*(availableFreeSpace+1)),SEEK_SET);
                    fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
                    
                    // Actualizamos la cantidad de registros en el header
                    newRecordCount++;
                    fseek(tarFile,0,SEEK_SET);
                    fwrite(&newRecordCount, sizeof(int), 1, tarFile);

                    // corregimos el offset al agregar un espacio vacio nuevo
                    recordCount++;
                }
            
            // El Ultimo caso seria donde no hay un espacio disponible por lo que hay que guardar el archivo actualizado
            // al final de los archivos
            } else {

                printf("no hay espacio disponible\n");

                // Agregamos el archivo al final
                fseek(tarFile,0,SEEK_END);
                fileHeaderRecord.start = ftell(tarFile);
                fwrite(buffer,newFileSize,1,tarFile);

                // Agregamos el nuevo record al header
                fileHeaderRecord.size = newFileSize;
                fileHeaderRecord.deleted = false;
                fseek(tarFile,sizeof(int)+(sizeof(fileHeaderRecord)*newRecordCount),SEEK_SET);
                fwrite(&fileHeaderRecord,sizeof(fileHeaderRecord),1,tarFile);
                // Guardamos la nueva cantidad de records
                newRecordCount++;
                fseek(tarFile,0,SEEK_SET);
                fwrite(&newRecordCount, sizeof(int), 1, tarFile);
            }

            printHeader(tarFile);

        // Si el tamaño del nuevo archivo es mas pequeño que el espacio del archivo anterior, entonces
        // hay que agregar un registro que indique el espacio sobrante vacio
        } else {
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


            printHeader(tarFile);
        }

        strcmp(parameters[i],""); // Eliminamos de la lista de parametros el archivo que acabamos de actualizar

        //liberar recursos
        free(buffer);
        fclose(currentUpdatedFile);
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
    } else if(update) {
        updateStar(argc,argv);
    }

    exit(0);
}