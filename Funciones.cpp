#include "Funciones.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
void Menu(){
    bool a = true;
    while(a){
        int b= 0;
        cout<<"------------------------------------------------------------------------\n\n\n";
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
            case 1:
                
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
            default:
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
    stringstream DBName;
    bool finish = false;
    system("clear");
    
    cout<<"------------------------------------------------------------------------\n\n\n";
    cout<<"Escriba el nombre de la Base de datos\n";
    cout<<"Nombre: "; cin>>Name;
    DBName<<Name.c_str()<<".dbf";
    cout<<"Escriba el tamaño de la base de datos\n";
    cout<<"Tamaño en Bytes: "; cin>>DBSize;
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
    
}

void CreateDBFile(const char*name,Metadatabase mtb,MetaDataBitMap mdbm,Blocks bl,int bc){
    ofstream BDfile(name,ios::out | ios::binary);
    BDfile.write(reinterpret_cast<const char*>(&mtb),sizeof(Metadatabase));
    BDfile.write(reinterpret_cast<const char*>(&mdbm),sizeof(MetaDataBitMap));

    for(int i = 0; i < bc; i++)
    {
        BDfile.write(reinterpret_cast<const char*>(&bl),bl.sizeB);
    }

    BDfile.close();
}


