#include "pch.h"
#include <iostream>
#include <mysql.h>
#include <iomanip>

using namespace std;
MYSQL* conector;
int status = 0;

struct producto {
    char producto[50], descripcion[100], fecha_in[15];
    int idMarca, existencia;
    float p_venta, p_costo;
};

struct marca {
    int idMarca;
    char marca[50];
};

void conexion_open_sql() {
    conector = mysql_init(0);
    conector = mysql_real_connect(conector, "::1", "db_c++", "dB@c++*_", "db_bodega", 3306, NULL, 0);
    status = 1;
}

void conexion_closed_sql() {
    mysql_close(conector);
    status = 0;
}

void revision_sql() {
    system("cls");
    if (status == 0) {
        conexion_open_sql();
    }
}

void ingreso(int tab) {
    revision_sql();
    if (tab == 1) {
        producto product;
        cout << "******************************" << endl;
        cout << "*****Ingreso de productos*****" << endl;
        cout << "******************************\n" << endl;
        cout << "Ingrese un nuevo producto: "<<endl;
        cout << "Nombre: ";
        cin >> product.producto;
        cout << "ID Marca: ";
        cin >> product.idMarca;
        cout << "Descripcion: ";
        //getline(product.descripcion);
        cin >> product.descripcion;
        cout << "Precio del costo: ";
        cin >> product.p_costo;
        cout << "Precio de venta: ";
        cin >> product.p_venta;
        cout << "Existencia: ";
        cin >> product.existencia;
        cout << "Fecha de ingreso: ";
        cin >> product.fecha_in;
        

        string str_producto(product.producto);
        string str_idmarca = to_string(product.idMarca);
        string str_descripcion(product.descripcion);
        string str_costo = to_string(product.p_costo);
        string str_venta = to_string(product.p_venta);
        string str_existencia = to_string(product.existencia);
        string str_fecha(product.fecha_in);


        string insert_dat = "insert into productos(producto, idMarca, descripcion, precio_costo, precio_venta, existencia, fecha_ingreso) values ('" + str_producto + "', " + str_idmarca + ", '" + str_descripcion + "', " + str_costo + ", " + str_venta + ", " + str_existencia + ", STR_TO_DATE('" + str_fecha + "', '%d-%m-%Y'))";
        const char* execute_insert = insert_dat.c_str();
        int q_estado = mysql_query(conector, execute_insert);
        if (!q_estado) {
            cout << "\n\nValor agregado a productos a las ";
            system("time /t");
        }
        else {
            cout << "\n\nIngreso F\n\n";
        }
        
    }
    else if (tab == 2) {
        marca marc;
        cout << "******************************" << endl;
        cout << "*******Ingreso de marcas******" << endl;
        cout << "******************************\n" << endl;
        cout << "Ingrese el nombre de la marca: ";
        cin >> marc.marca;

        string str_marca(marc.marca);

        string insert_dat = "insert into marcas(marca) values ('" + str_marca + "')";
        const char* execute_insert = insert_dat.c_str();
        int q_estado = mysql_query(conector, execute_insert);
        if (!q_estado) {
            cout << "\n\nValor agregado a marcas a las ";
            system("time /t");
        }
        else {
            cout << "\n\nIngreso invalido, sirvase a intentar de nuevo\n\n";
        }
    }
    else {
        cout << "¡Referencia de tabla incoherente!";
    }
    system("pause");
    system("cls");
    conexion_closed_sql();
}

void mostrar(int tab) {
    revision_sql();
    MYSQL_ROW fila;
    MYSQL_RES* result;
    if (tab == 1) {
        string show_dat = "select * from productos";
        const char* execute_show = show_dat.c_str();
        int q_estado = mysql_query(conector, execute_show);
        if (!q_estado) {
            cout << "*********************************************************************************************************************" << endl;
            cout << "*******************************************************PRODUCTOS*****************************************************"<<endl;
            cout << "*********************************************************************************************************************" << endl;
            cout << "No. | ID |  Producto  | MarcaID |    Descripcion   | Precio Costo | Precio Venta | Existencia |   Fecha de ingreso  |" << endl;
            cout << "----+----+------------+---------+------------------+--------------+--------------+------------+---------------------+" << endl;
            int i = 1;
            result = mysql_store_result(conector);
            while (fila = mysql_fetch_row(result)) {
                cout << i << setw(5) << " | " << setw(2) << fila[0] << " | " << setw(10) << fila[1] << " | " << setw(7) << fila[2] << " | " << setw(16) << fila[3] << " | " << setw(12) << fila[4] << " | " << setw(12) << fila[5] << " | " << setw(10) << fila[6] << " | " << fila[7] << " | " << endl;
                i++;
            }
        }
        else {
            cout << "\n\nMostrar F\n\n";
        }
    }
    else if (tab == 2) {
        string show_dat = "select * from marcas";
        const char* execute_show = show_dat.c_str();
        int q_estado = mysql_query(conector, execute_show);
        if (!q_estado) {
            cout << "***********************************" << endl;
            cout << "**************MARCAS***************" << endl;
            cout << "***********************************" << endl;
            cout << "No. | ID Marca |       Marca      |" << endl;
            cout << "----+----------+------------------+" << endl;
            int i = 1;
            result = mysql_store_result(conector);
            while (fila = mysql_fetch_row(result)) {
                cout << i << setw(5) << " | " << setw(8) << fila[0] << " | " << setw(16) << fila[1] << " | "<<endl;
                i++;
            }
        }
        else {
            cout << "\n\nError al mostrar, sirvase a intentar de nuevo\n\n";
        }
    }
    system("pause");
    conexion_closed_sql();
}

void eliminar() {
    int int_tabla;
    string delete_dat, str_id;
    cout << "Seleccione servicio: " << endl;
    cout << "Productos ........ 1" << endl;
    cout << "Marca ............ 2" << endl;
    cout << "\nIngrese servicio: ";
    cin >> int_tabla;
    mostrar(int_tabla);
    cout << "\n\n\n\nIngrese el ID para eliminar: ";
    cin >> str_id;
    revision_sql();
    if (int_tabla == 1) {
        delete_dat = "delete from productos where idProducto=" + str_id;
    }
    else if (int_tabla == 2) {
        delete_dat = "delete from marcas where idMarca=" + str_id;
    }
    
    const char* execute_delete = delete_dat.c_str();
    int q_estado = mysql_query(conector, execute_delete);
    if (!q_estado) {
        cout << "Eliminado con exito...!\n\n";
        mostrar(int_tabla);
    }
    else {
        cout << "\n\nF\n\n";
    }
    system("pause");
    system("cls");
}

void update() {
    int int_tabla;
    string update_dat, str_id, str_new, str_column;
    cout << "Seleccione servicio: " << endl;
    cout << "Productos ........ 1" << endl;
    cout << "Marca ............ 2" << endl;
    cout << "\nIngrese servicio: ";
    cin >> int_tabla;
    mostrar(int_tabla);
    cout << "\n\n\n\nIngrese el ID para alterar: ";
    cin >> str_id;
    cout << "Ingresa el campo a editar: ";
    cin >> str_column;
    cout << "Ingrese el nuevo valor: ";
    cin >> str_new;
    revision_sql();
    if (int_tabla == 1) {
        update_dat = "update productos set " + str_column + "='" + str_new + "' where idProducto=" + str_id;
    }
    else if (int_tabla == 2) {
        update_dat = "update marcas set " + str_column + "='" + str_new + "' where idMarca=" + str_id;
    }

    const char* execute_update = update_dat.c_str();
    int q_estado = mysql_query(conector, execute_update);
    if (!q_estado) {
        cout << "\n\Valor actualizado a las ";
        system("time /t");
    }
    else {
        cout << "\n\nError, sirvase a intentar de nuevo\n\n";
    }
    system("pause");
    system("cls");
    conexion_closed_sql();
}

void banner_motd() {
    system("cls");
    system("date /t");
    cout << "\n\n   _|_|_|  _|_|_|    _|    _|  _|_|_|   " << endl;
    cout << " _|        _|    _|  _|    _|  _|    _| " << endl;
    cout << " _|        _|_|_|    _|    _|  _|    _|  " << endl;
    cout << " _|        _|    _|  _|    _|  _|    _| " << endl;
    cout << "   _|_|_|  _|    _|    _|_|    _|_|_|  \n" << endl;
}

int main()
{
    int int_fin = 0;
    do {
        int int_menu, int_op;
        banner_motd();
        cout << "Agregar ............................. 1" << endl;
        cout << "Mostrar ............................. 2" << endl;
        cout << "Actualizar .......................... 3" << endl;
        cout << "Eliminar ............................ 4" << endl;
        cout << "Salir ............................... 5" << endl;
        cout << "\n  Bienvenido, escoge una opcion:";
        cin >> int_menu;
        system("cls");
        switch (int_menu)
        {
        case 1:
            banner_motd();
            cout << "Productos ........................... 1" << endl;
            cout << "Marcas .............................. 2" << endl;
            cout << "\n          Ingresa una opcion: ";
            cin >> int_op;
            ingreso(int_op);
            break;
        case 2:
            banner_motd();
            cout << "Productos ........................... 1" << endl;
            cout << "Marcas .............................. 2" << endl;
            cout << "\n          Ingresa una opcion: ";
            cin >> int_op;
            mostrar(int_op);
            break;
        case 3:
            update();
            break;
        case 4:
            eliminar();
            break;
        case 5:
            int_fin = 1;
            cout << "Hora de salida ";
            system("time /t");
            break;
        default:
            cout << "Opcion no valida, sirvase a intentar de nuevo";
            break;
        }
    } while (int_fin == 0);
}

