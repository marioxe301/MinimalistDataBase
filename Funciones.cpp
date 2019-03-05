#include "Funciones.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
static string ActualDB;
void Menu(){
    bool a = true;
    while(a){
        int b= 0;
        cout<<"------------------------------------------------------------------------\n\n\n";
        cout<<"0.Seleccionar Base de Datos\n";
        cout<<"1.Crear Base de datos\n";
        cout<<"2.Eliminar Base de dato\n";
        cout<<"3.Insertar datos a Tabla\n";
        cout<<"4.Borrar datos de Tabla\n";
        cout<<"5.Update datos de tabla\n";
        cout<<"6.Select datos de Tabla\n";
        cout<<"7.Salir\n";
        cin>>b;

        switch (b)
        {
            case 0:
                ChangeDataBase();
                break;
            case 1:
                CreateDatabase();

                break;
            case 2:

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
    DBName<<Name.c_str()<<".txt";
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

    MetadaUpdate(DBName.str().c_str());    

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

    for(int i = 0; i < bc; i++)
    {
        BDfile.write(reinterpret_cast<const char *>(&bl.size),sizeof(int));
        BDfile.write(&bl.Data[0],sizeof(char)*bl.size);
        BDfile.write(reinterpret_cast<const char*>(&bl.nextBlock),sizeof(int));
    }

    BDfile.close();
}

void MetadaUpdate(const char*name){
    int TBSP = TableSpaceP(name);
    int FTBSP = FreeTableSpace(name);
    int FBS = FreeBlockSpace(name);
    int FB = FreeBlock(name);

    ofstream BDfile(name,ios::out | ios::binary);
    if(!BDfile){
      cout<<"Error al abrir el archivo"<<endl;  
    }

    BDfile.seekp(0,ios::beg);

    BDfile.seekp(sizeof(Metadatabase));

    BDfile.write(reinterpret_cast<const char *>(&FTBSP),sizeof(int));
    BDfile.write(reinterpret_cast<const char*>(&TBSP),sizeof(int));
    BDfile.write(reinterpret_cast<const char*>(&FB),sizeof(int));
    BDfile.write(reinterpret_cast<const char*>(&FBS),sizeof(int));

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

void CreateTable(const char*name){
    int pos = FreeTableSpace(name);
    int cantidadColu = 0;
    char name[30]="";

    MetaDataTables mT;
    ofstream BDfile(name,ios::out | ios::binary);
    BDfile.seekp(pos);
    


}