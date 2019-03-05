#ifndef F_H
#define F_H 

struct Metadatabase 
{
    int size; //tamaño default 1MB
    int BlockSize;
    int NumberBlocks;
};

struct MetaDataBitMap{ //Metadata de Bloques Libres
    int TablesSpace; //Espacio de tabla libre
    int MetaTablesP; //puntero al metadata de tablas
    int FreeBlock; //Posision de bloque libre Bytes
    int FreeSpaceBlock; //Espacio disponible del bloque
};

struct MetaDataTables
{
    char name[30]; //nombre de la tabla
    int DataPointer; //poscicion en bytes de los registros
    int ColumnCount; // cantidad de columnas  
    int RegisterSize; // tamaño del registros
    bool Deleted; // indicador de borrado de tabla
};

/*struct TableReference{
    char TnameReference[30];
};*/

struct MetaDataColumn{
    char nameT[30]; // nombre de la tabla refereciada
    char datatype; // tipo de datos  i ó d ó v
    int varSize; // opcional si hay un tipo char max 
};

struct Registers{
    int NextData;
    bool Deleted;
    char *Data; //Data de la tabla
};

struct Blocks{
    int size;
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
void MetadaUpdate(const char*name);
int FreeBlock(const char*name);
int FreeBlockSpace(const char*name);
int TableSpaceP(const char*name);
int FreeTableSpace(const char*name);
int DataBaseTable(const char*name);

void CreateTable(const char*name);

void ChangeDataBase();
bool ExistDataBase(const char*name);


void ListTables(const char*name);
void ListTablesColumns(const char*name);
void UpdateRegisterSize(const char*name);

#endif // !F_H
