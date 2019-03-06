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

void Menu();//t
void CreateDatabase(); //t
void DropDatabase(const char *name); //t

void Insert();
void Delete();
void Update();
void Select();

void CreateDBFile(const char*name,Metadatabase mtb,MetaDataBitMap mdbm,Blocks bl,int bc);//t
void MetadaUpdate(const char*name);//t
int FreeBlock(const char*name);//t
int FreeBlockSpace(const char*name);//t
int TableSpaceP(const char*name); //t
int FreeTableSpace(const char*name); //t
void ListTables(const char*name);//t

//int DataBaseTable(const char*name); //t

void CreateTable(const char*name);//
void DropTable(const char*name);


void ChangeDataBase();//t
bool ExistDataBase(const char*name);//t


void ListTablesColumns(const char*name);
#endif // !F_H
