#include "Funciones.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;
static string ActualDB;
static string Table;
void Menu(){
    bool a = true;
    while(a){
        
        int b= 0;
        int c=0;
        string db;
        cout<<"------------------------------------------------------------------------\n\n\n";
        cout<<"0.Seleccionar Base de Datos\n";
        cout<<"1.Crear Base de datos\n";
        cout<<"2.Eliminar Base de datos\n";
        cout<<"3.Insertar datos a Tabla\n";
        cout<<"4.Borrar datos de Tabla\n";
        cout<<"5.Update datos de tabla\n";
        cout<<"6.Select datos de Tabla\n";
        cout<<"7.Crear Tabla\n";
        cout<<"8.Eliminar Tabla\n";
        cout<<"9.Listar Tablas\n";
        cout<<"10.Ver Database actual\n";
        cout<<"11.Salir\n";
        cin>>b;

        switch (b)
        {
            case 0:
                ChangeDataBase();
                break;
            case 1:
                CreateDatabase();

                MetadaUpdate(ActualDB.c_str());
                break;
            case 2:
                system("clear");
        
                cout<<"Escriba el nombre de la base de dato a borrar: ";cin>>db;
                DropDatabase(db.c_str());

                break;
            case 3:
                break;
            case 4:
                
                break;
            case 5:
                
                break;
            case 6:
                
                break;
            case 7:
                CreateTable(ActualDB.c_str());

                MetadaUpdate(ActualDB.c_str());

                break;
            case 8:
                c = PosTable(ActualDB.c_str());
                DropTable(ActualDB.c_str(),c);

                break;
            case 9:
                ListTables(ActualDB.c_str());
                break;
            case 10:
                system("clear");
                cout<<"Escriba a que base de datos quiere moverse: "; cin>>ActualDB;
                if(ExistDataBase(ActualDB.c_str())){
                    cout<<"Base de datos actual: "<<ActualDB<<"\n";
                }else{
                    cout<<"No existe la base de datos\n";
                }
                break;
            case 11:
                a=false;
                break;
            default:
                cout<<"Opcion no valida\n";
                break;
        }

    }
}

void CreateDatabase(){
    Metadatabase mtd;
    MetaDataBitMap mdbm;
    Blocks bl;

    int BlockSize=0;
    int BlockCount=0;
    string Name;
    int DBSize =0;
    int DBConv=0;
    stringstream DBName;
    bool finish = false;
    system("clear");
    
    cout<<"------------------------------------------------------------------------\n\n\n";
    cout<<"Escriba el nombre de la Base de datos\n";
    cout<<"Nombre: "; cin>>Name;
    DBName<<Name.c_str();
    cout<<"Escriba el tamaño de la base de datos\n";
    cout<<"Tamaño en MB: "; cin>>DBSize;
    cout<<"Escriba el tamaño de los bloques\n";
    bool b=true;    
    while(b){
    cout<<"Tamaño en Bytes:";cin>>BlockSize;
        if(BlockSize>=512 && BlockSize<=8192){
            b= false;
            
        }else{
            cout<<"Tamaño fuera del rango(512-8192) bytes\n";
        }
    
    }
    //Conversion de MB a BY
    DBConv = DBSize *1048576;
    //Cantidad de bloques
    BlockCount = DBConv/BlockSize;
    mtd.BlockSize = BlockSize; //tamaño en bytes
    mtd.NumberBlocks = BlockCount;
    mtd.size = DBSize; // tamaño en MB


    mdbm.TablesSpace = 0;
    mdbm.FreeBlock = 0;
    mdbm.FreeSpaceBlock = BlockSize;
    mdbm.MetaTablesP = 0;

    bl.size= BlockSize;
    bl.Data = new char[BlockSize];
    strcpy(bl.Data,"");
    bl.nextBlock = 0;
    
    CreateDBFile(DBName.str().c_str(),mtd,mdbm,bl,BlockCount);

    //MetadaUpdate(DBName.str().c_str());    

    ActualDB = DBName.str().c_str();

    cout<<"Base de Datos creada"<<endl;
    cout<<"Cantidad de Bloques: "<<BlockCount<<endl;

}

void CreateDBFile(const char*name,Metadatabase mtb,MetaDataBitMap mdbm,Blocks bl,int bc){
    ofstream BDfile(name,ios::out | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }
    BDfile.write(reinterpret_cast<const char*>(&mtb),sizeof(Metadatabase));
    BDfile.write(reinterpret_cast<const char*>(&mdbm),sizeof(MetaDataBitMap));
    int a = BDfile.tellp();
    for(int i = 0; i < bc; i++)
    {
        BDfile.write(reinterpret_cast<const char *>(&bl.size),sizeof(int));
        BDfile.write(&bl.Data[0],sizeof(char)*bl.size);
        BDfile.write(reinterpret_cast<const char*>(&bl.nextBlock),sizeof(int));
        a = BDfile.tellp();
    }    

    BDfile.close();
}

void MetadaUpdate(const char*name){
    int TBSP = TableSpaceP(name);
    int FTBSP = FreeTableSpace(name);
    int FBS = FreeBlockSpace(name);
    int FB = FreeBlock(name);

    MetaDataBitMap mtbm;
    ofstream BDfile(name,ios::out|ios::in| ios::binary );
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }

    BDfile.seekp(0,ios::beg);

    BDfile.seekp(sizeof(Metadatabase));
    mtbm.FreeBlock = FB;
    mtbm.FreeSpaceBlock= FBS;
    mtbm.TablesSpace = FTBSP;
    mtbm.MetaTablesP = TBSP;

    BDfile.write(reinterpret_cast<const char *>(&mtbm),sizeof(MetaDataBitMap));
    BDfile.close();

}

int FreeBlock(const char*name){
    ifstream BDfile(name,ios::in | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }

    Metadatabase mtb;
    Blocks bl;
    
    BDfile.read(reinterpret_cast<char*>(&mtb),sizeof(Metadatabase));
    BDfile.seekg(0,ios::beg);
    BDfile.seekg(sizeof(Metadatabase)+sizeof(MetaDataBitMap));
    
    int a = 0;

    for(int i=0;i<mtb.NumberBlocks;i++){

        a = BDfile.tellg();
        BDfile.read(reinterpret_cast<char*>(&bl.size),sizeof(int));
        bl.Data = new char[mtb.BlockSize];
        BDfile.read(reinterpret_cast<char*>(&bl.Data[0]),mtb.BlockSize);
        BDfile.read(reinterpret_cast<char*>(&bl.nextBlock),sizeof(int));

        if(strcmp(&bl.Data[mtb.BlockSize-1],"")==0){
            BDfile.close();
            return a;
        }
        
    }
        BDfile.close();
        return 0;
}

int FreeBlockSpace(const char*name){
    int pos = FreeBlock(name);

    ifstream BDfile(name,ios::in | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }
    Metadatabase mtb;
    Blocks bl;
    int space=0;
    int position=0;
    BDfile.read(reinterpret_cast<char*>(&mtb),sizeof(Metadatabase));
    BDfile.seekg(0,ios::beg);

    BDfile.seekg(pos);
    BDfile.read(reinterpret_cast<char*>(&bl.size),sizeof(int));

    bl.Data = new char[mtb.BlockSize];
    BDfile.read(reinterpret_cast<char*>(&bl.Data[0]),sizeof(char)*mtb.BlockSize);
    BDfile.read(reinterpret_cast<char*>(&bl.nextBlock),sizeof(int));

    cout<<"prueba: "<<&bl.Data[0]<<endl;

    space = strlen(&bl.Data[0]);
    position = mtb.BlockSize - space;
    BDfile.close();

    return position;
    
}

int TableSpaceP(const char*name){
    ifstream BDfile(name,ios::in | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }
    Metadatabase mtb;
    int position=0;
    int a =0;
    BDfile.read(reinterpret_cast<char*>(&mtb),sizeof(Metadatabase));
    BDfile.seekg(0,ios::beg);
    BDfile.seekg(sizeof(Metadatabase)+sizeof(MetaDataBitMap));
    a = BDfile.tellg(); //Solo para Debug
    BDfile.seekg((mtb.BlockSize+8)*mtb.NumberBlocks);
    
    position = BDfile.tellg();
    position +=28;
    BDfile.close();

    return position;
}

int FreeTableSpace(const char*name){
    ifstream BDfile(name,ios::in | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }

    Metadatabase mtb;
    int position=0;

    BDfile.seekg(0,ios::end);

    position = BDfile.tellg();
    BDfile.close();

    return position;
}

void ChangeDataBase(){
    system("clear");
    bool a = true;
    string dbn;
    cout<<"----------------------------Seleccione la base de datos----------------------------\n\n";
    while(a){
        cout<<"Seleccione su base de datos: "; cin>>dbn;
        cout<<"\n";
        if(ExistDataBase(dbn.c_str())){
            a=false;
        }
    }
    

}

bool ExistDataBase(const char*name){
    ifstream BDfile(name,ios::in | ios::binary);

    if(BDfile.good()){
        BDfile.close();
        return true;
    }else{
        BDfile.close();
        return false;
    }
}

void CreateTable(const char*name)
{
    system("clear");
    int pos = FreeTableSpace(name);
    int RegisterSize=0;
    MetaDataTables mT;
    MetaDataColumn mC;
    string datatype;

    ofstream BDfile(name,ios::out|ios::in|ios::binary|ios::app);
    //BDfile.seekp(pos);
    
    cout<<"Ingrese el nombre de la tabla: "; cin>>mT.name;
    cout<<"Cantidad de columnas a ingresar: ";cin>>mT.ColumnCount;
    mT.Deleted = false;
    mT.RegisterSize = 0;
    int a = 0;

    BDfile.write(reinterpret_cast<const char*>(&mT),sizeof(MetaDataTables));

    while(a<mT.ColumnCount){
        cout<<"Ingrese el tipo de dato de la columna "<<a+1<<" \n";
        cout<<"Tipos de datos: int, double, varchar\n";
        cin>>datatype;

        if(datatype=="int"){
            cout<<"Nombre de la columna: ";cin>>mC.nameT;
            mC.varSize=0;
            mC.datatype = 'i';
            BDfile.write(reinterpret_cast<const char*>(&mC),sizeof(MetaDataColumn));
            RegisterSize+=4;
            a++;    
        }else if(datatype=="double"){
            cout<<"Nombre de la columna: ";cin>>mC.nameT;
            mC.varSize=0;
            mC.datatype = 'd';
            BDfile.write(reinterpret_cast<const char*>(&mC),sizeof(MetaDataColumn));
            RegisterSize+=8;
            a++;
        }else if(datatype=="varchar"){
            cout<<"Nombre de la columna: ";cin>>mC.nameT;
            cout<<"Tamaño del varchar(1-4000): "; cin>>mC.varSize;
            mC.datatype = 'v';
            BDfile.write(reinterpret_cast<const char*>(&mC),sizeof(MetaDataColumn));
            RegisterSize+=mC.varSize;
            a++;
        }else
        {
            cout<<"Tipo de dato incorrecto\n";
        }
        
    }
        /*BDfile.seekp(pos);
        cout<<"Tamaño total del registro: "<< RegisterSize<<endl;

        mT.RegisterSize=RegisterSize;

        BDfile.write(reinterpret_cast<const char*>(&mT),sizeof(MetaDataTables));*/
        BDfile.close();

        ofstream Test(name,ios::in |ios::out |ios::binary);
        Test.seekp(pos);
        mT.RegisterSize = RegisterSize;
        cout<<"Tamaño total del registro: "<< RegisterSize<<endl;
        Test.write(reinterpret_cast<const char*>(&mT),sizeof(MetaDataTables));
        Test.close();
}



void ListTables(const char*name){
    system("clear");
    int pos = TableSpaceP(name);
    int posfinal = FreeTableSpace(name);
    MetaDataTables mT;
    MetaDataColumn mC;
    

    ifstream BDfile(name,ios::in | ios::binary);
    BDfile.seekg(pos);
    int a = BDfile.tellg();
    
    while(a<posfinal){

    BDfile.read(reinterpret_cast<char*>(&mT),sizeof(MetaDataTables));

    if(mT.Deleted==false){
        cout<<"Tabla ubicada en el byte: "<<a<<" Tabla: "<<mT.name<<"\n";

        for(int i=0;i<mT.ColumnCount;i++){

            BDfile.read(reinterpret_cast<char*>(&mC),sizeof(MetaDataColumn));

            if(mC.datatype=='i'|| mC.datatype=='d'){
                cout<<"Columna: "<<mC.nameT<<"\n";
                cout<<"Tipo de Dato: "<<mC.datatype<<"\n";
                cout<<endl;
            }else{
                cout<<"Columna: "<<mC.nameT<<"\n";
                cout<<"Tipo de Dato: "<<mC.datatype<<"\n";
                cout<<"Tamaño de varchar: "<<mC.datatype<<"\n";
                cout<<endl;
            }
        }
    }

    a = BDfile.tellg();
    //cout<<"";
    }

    BDfile.close();

}

void DropDatabase(const char *name){
    system("clear");
    if(ExistDataBase(name)){
    remove(name);
    }else
    {
        cout<<"No existe la base de datos a borrar\n";
    }
    
}

void DropTable(const char*name,int pos){
    MetaDataTables mt = TableReturn(name,pos);

    ofstream BDfile(name,ios::out|ios::in|ios::binary);
    BDfile.seekp(pos);

    mt.Deleted = true;

    BDfile.write(reinterpret_cast<char*>(&mt),sizeof(MetaDataTables));
    BDfile.close();


}

int PosTable(const char*name){
    int pos = TableSpaceP(name);
    int posfinal = FreeTableSpace(name);
    char table[30];

    cout<<"Escriba el nombre de la tabla: ";cin>>table;

    ifstream BDfile(name,ios::in|ios::binary);
    MetaDataTables mT;
    MetaDataColumn mC;
    BDfile.seekg(pos);

    int a = BDfile.tellg();

    while(a<posfinal){
        BDfile.read(reinterpret_cast<char*>(&mT),sizeof(MetaDataTables));
        if(mT.Deleted==false){
            if(mT.name==table){
                a = BDfile.tellg();
                BDfile.close();
                return a;
            }

            for(int i=0;i<mT.ColumnCount;i++){
                BDfile.read(reinterpret_cast<char*>(&mC),sizeof(MetaDataColumn));
            }
        }
        a = BDfile.tellg();
    }
    BDfile.close();
    return -1;
}

MetaDataTables TableReturn(const char* name,int pos){
    ifstream BDfile(name,ios::in|ios::binary);
    MetaDataTables mT;
    BDfile.seekg(pos);

    BDfile.read(reinterpret_cast<char*>(&mT),sizeof(MetaDataTables));
    BDfile.close();
    return mT;
}



