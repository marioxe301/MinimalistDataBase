#ifndef F_H
#define F_H 

struct Metadatabase 
{
    int size; //tamaño default 1MB
    int MetaTablesP; //puntero al metadata de tablas
    int BlockSize;
    int NumberBlocks;
};

struct MetaDataBitMap{ //Metadata de Bloques Libres
    int FreeBlock; //Posision de bloque libre Bytes
    int DataPointerFreeR; //Pos de bloque con espacion aun disponible
};

struct MetaDataTables
{
    char name[30]; //nombre de la tabla
    int DataPointer; //poscicion en bytes de los registros
    int ColumnPointer; // poscicion de la columna en archivo externo 
    int RegisterSize;
};

struct MetaDataColumn{
    char nameT[30]; // nombre de la tabla refereciada
    char datatype; // tipo de datos  i ó d ó v
    int varSize; // opcional si hay un tipo char max 
};

struct Registers{
    char *Data; //Data de la tabla
    int NextData;
};

struct Blocks{
    int sizeB;
    char *Data; // bloque de dato
    int nextBlock; // apuntador al siguiente bloque disponible en caso de que se llene el DB
};

void Menu();
void CreateDatabase();
void DropDatabase();
void CreateTable();
void DropTable();
void Insert();
void Delete();
void Update();
void Select();

void CreateDBFile(const char*name,Metadatabase mtb,MetaDataBitMap mdbm,Blocks bl,int bc);

#endif // !F_H
