#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h> 

// --- CODIGOS DE COLOR ANSI ---
#define C_RESET   "\x1b[0m"
#define C_ROJO    "\x1b[31m"
#define C_VERDE   "\x1b[32m"
#define C_AMARILLO "\x1b[33m"
#define C_AZUL    "\x1b[34m"
#define C_CIAN    "\x1b[36m"

// --- ESTRUCTURAS Y CONSTANTES ---
#define NUM_GRUPOS 12
#define EQUIPOS_POR_GRUPO 4
#define FIGURAS_POR_EQUIPO 20
#define ARCHIVO_DAT "album.dat"

typedef struct { int estado; } Figurita;
typedef struct {
    char nombre[40];
    char codigo[5];
    Figurita figuras[FIGURAS_POR_EQUIPO];
} Equipo;
typedef struct {
    char letra;
    Equipo equipos[EQUIPOS_POR_GRUPO];
} Grupo;

Grupo album[NUM_GRUPOS];
Equipo fwc;

// --- PROTOTIPOS ---
void inicializarAlbum();
void guardarAlbum();
void cargarAlbum();
void mostrarResumen();
void mostrarEstadisticas();
void cargarPorCodigo();
void limpiarPantalla();
void gestionarEquipoMenu(Equipo* eq);
void registrarMovimiento(const char* codigoEquipo, int numero, const char* accion);
int leerEntero();

// Funciones Helper (Optimizacion)
Equipo* buscarEquipo(const char* codigo);
void procesarEstadisticasEquipo(Equipo* eq, int* obtenidas, int* repetidas);

// --- FUNCIONES ---

void limpiarPantalla() { system("cls"); }

int leerEntero() {
    char buffer[20];
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

void registrarMovimiento(const char* codigoEquipo, int numero, const char* accion) {
    FILE* logFile;
    fopen_s(&logFile, "historial.log", "a");

    if (logFile != NULL) {
        time_t t = time(NULL);
        struct tm tm_info;
        localtime_s(&tm_info, &t);

        fprintf(logFile, "[%02d/%02d/%04d %02d:%02d:%02d] %s - %s #%d\n",
            tm_info.tm_mday, tm_info.tm_mon + 1, tm_info.tm_year + 1900,
            tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec,
            accion, codigoEquipo, numero);

        fclose(logFile);
    }
}

Equipo* buscarEquipo(const char* codigo) {
    if (_stricmp(fwc.codigo, codigo) == 0) return &fwc;

    for (int i = 0; i < NUM_GRUPOS; i++) {
        for (int j = 0; j < EQUIPOS_POR_GRUPO; j++) {
            if (_stricmp(album[i].equipos[j].codigo, codigo) == 0) {
                return &album[i].equipos[j];
            }
        }
    }
    return NULL;
}

void procesarEstadisticasEquipo(Equipo* eq, int* obtenidas, int* repetidas) {
    *obtenidas = 0;
    if (repetidas) *repetidas = 0;

    for (int k = 0; k < FIGURAS_POR_EQUIPO; k++) {
        if (eq->figuras[k].estado > 0) {
            (*obtenidas)++;
            if (repetidas && eq->figuras[k].estado > 1) {
                (*repetidas) += (eq->figuras[k].estado - 1);
            }
        }
    }
}

void mostrarEstadisticas() {
    int totalFiguras = (NUM_GRUPOS * EQUIPOS_POR_GRUPO * FIGURAS_POR_EQUIPO) + FIGURAS_POR_EQUIPO;
    int obtenidas = 0, repetidas = 0;
    int obsEq = 0, repEq = 0;

    for (int i = 0; i < NUM_GRUPOS; i++) {
        for (int j = 0; j < EQUIPOS_POR_GRUPO; j++) {
            procesarEstadisticasEquipo(&album[i].equipos[j], &obsEq, &repEq);
            obtenidas += obsEq;
            repetidas += repEq;
        }
    }

    procesarEstadisticasEquipo(&fwc, &obsEq, &repEq);
    obtenidas += obsEq;
    repetidas += repEq;

    float porcentaje = ((float)obtenidas / totalFiguras) * 100;
    printf("\n" C_CIAN "--- ESTADISTICAS DEL ALBUM ---" C_RESET "\n");
    printf(" Figuritas Totales: %d/%d (" C_AMARILLO "%.2f%%" C_RESET ")\n", obtenidas, totalFiguras, porcentaje);
    printf(" Figuritas Repetidas: " C_AZUL "%d" C_RESET "\n", repetidas);
    printf(C_CIAN "============================================" C_RESET "\n");
}

void mostrarResumen() {
    printf("\n" C_CIAN "============================================" C_RESET "\n");
    printf(C_CIAN "\tRESUMEN DE GRUPOS Y SELECCIONES" C_RESET "\n");
    printf(C_CIAN "============================================" C_RESET "\n");

    int obsEq = 0;

    for (int i = 0; i < NUM_GRUPOS; i++) {
        int obtenidasGrupo = 0;

        for (int j = 0; j < EQUIPOS_POR_GRUPO; j++) {
            procesarEstadisticasEquipo(&album[i].equipos[j], &obsEq, NULL);
            obtenidasGrupo += obsEq;
        }
        float pctGrupo = ((float)obtenidasGrupo / (EQUIPOS_POR_GRUPO * FIGURAS_POR_EQUIPO)) * 100;

        printf("Grupo %c [" C_AMARILLO "%.2f%%" C_RESET "]\n", album[i].letra, pctGrupo);

        for (int j = 0; j < EQUIPOS_POR_GRUPO; j++) {
            procesarEstadisticasEquipo(&album[i].equipos[j], &obsEq, NULL);
            float pctEquipo = ((float)obsEq / FIGURAS_POR_EQUIPO) * 100;
            printf("\t%d. %-20s (%s) [" C_AMARILLO "%.2f%%" C_RESET "]\n", j + 1, album[i].equipos[j].nombre, album[i].equipos[j].codigo, pctEquipo);
        }
    }

    procesarEstadisticasEquipo(&fwc, &obsEq, NULL);
    float pctFWC = ((float)obsEq / FIGURAS_POR_EQUIPO) * 100;

    printf("\n" C_CIAN "Figuritas Institucionales:" C_RESET "\n");
    printf("\t- %-20s (%s) [" C_AMARILLO "%.2f%%" C_RESET "]\n", fwc.nombre, fwc.codigo, pctFWC);
}

void inicializarAlbum() {
    char gruposLetras[] = "ABCDEFGHIJKL";
    const char* nombres[] = {
        "Mexico", "Sudafrica", "Corea del Sur", "Chequia", "Canada", "Bosnia y Herzegovina", "Qatar", "Suiza",
        "Brasil", "Marruecos", "Haiti", "Escocia", "EE.UU.", "Paraguay", "Australia", "Turquia",
        "Alemania", "Curazao", "Costa de Marfil", "Ecuador", "Paises Bajos", "Japon", "Suecia", "Tunez",
        "Belgica", "Egipto", "Iran", "Nueva Zelanda", "Espana", "Cabo Verde", "Arabia Saudita", "Uruguay",
        "Francia", "Senegal", "Irak", "Noruega", "Argentina", "Algeria", "Austria", "Jordania",
        "Portugal", "RD Congo", "Uzbekistan", "Colombia", "Inglaterra", "Croacia", "Ghana", "Panama"
    };
    const char* codigos[] = {
        "MEX", "RSA", "KOR", "CZE", "CAN", "BIH", "QAT", "SUI", "BRA", "MAR", "HAI", "SCO",
        "USA", "PAR", "AUS", "TUR", "GER", "CUW", "CIV", "ECU", "NED", "JPN", "SWE", "TUN",
        "BEL", "EGY", "IRN", "NZL", "ESP", "CPV", "KSA", "URU", "FRA", "SEN", "IRQ", "NOR",
        "ARG", "ALG", "AUT", "JOR", "POR", "COD", "UZB", "COL", "ENG", "CRO", "GHA", "PAN"
    };
    int idx = 0;
    for (int i = 0; i < NUM_GRUPOS; i++) {
        album[i].letra = gruposLetras[i];
        for (int j = 0; j < EQUIPOS_POR_GRUPO; j++) {
            strcpy_s(album[i].equipos[j].nombre, 40, nombres[idx]);
            strcpy_s(album[i].equipos[j].codigo, 5, codigos[idx]);
            for (int k = 0; k < FIGURAS_POR_EQUIPO; k++) album[i].equipos[j].figuras[k].estado = 0;
            idx++;
        }
    }

    strcpy_s(fwc.nombre, 40, "FWC");
    strcpy_s(fwc.codigo, 5, "FWC");
    for (int k = 0; k < FIGURAS_POR_EQUIPO; k++) fwc.figuras[k].estado = 0;
}

void guardarAlbum() {
    FILE* fp;
    fopen_s(&fp, ARCHIVO_DAT, "wb");
    if (fp != NULL) {
        fwrite(album, sizeof(album), 1, fp);
        fwrite(&fwc, sizeof(fwc), 1, fp);
        fclose(fp);
    }
}

void cargarAlbum() {
    FILE* fp;
    if (fopen_s(&fp, ARCHIVO_DAT, "rb") == 0) {
        fread(album, sizeof(album), 1, fp);
        if (fread(&fwc, sizeof(fwc), 1, fp) != 1) {
            strcpy_s(fwc.nombre, 40, "FWC");
            strcpy_s(fwc.codigo, 5, "FWC");
            for (int k = 0; k < FIGURAS_POR_EQUIPO; k++) fwc.figuras[k].estado = 0;
        }
        fclose(fp);
    }
    else { inicializarAlbum(); }
}

void cargarPorCodigo() {
    char codInput[10]; int num;
    printf("\nIngrese codigo (ej: ARG o FWC): ");
    fgets(codInput, 10, stdin);
    codInput[strcspn(codInput, "\n")] = 0;

    printf("Ingrese numero (1-20): ");
    num = leerEntero();
    if (num < 1 || num > 20) return;

    Equipo* eq = buscarEquipo(codInput);
    if (eq != NULL) {
        eq->figuras[num - 1].estado++;
        registrarMovimiento(eq->codigo, num, "CARGA RAPIDA");
        printf(C_VERDE "\n>> Figurita cargada con exito!" C_RESET "\n");
    }
    else {
        printf(C_ROJO "\n>> Codigo no encontrado." C_RESET "\n");
    }
}

void gestionarEquipoMenu(Equipo* eq) {
    char inputBuffer[10];
    while (1) {
        limpiarPantalla();
        printf("\n" C_CIAN "--- FIGURAS: %s (%s) ---" C_RESET "\n", eq->nombre, eq->codigo);
        for (int i = 0; i < FIGURAS_POR_EQUIPO; i++) {
            int st = eq->figuras[i].estado;

            if (st == 0) {
                printf(C_ROJO "[  ]" C_RESET " %-2d  ", i + 1);
            }
            else if (st == 1) {
                printf(C_VERDE "[OK]" C_RESET " %-2d  ", i + 1);
            }
            else {
                printf(C_AZUL "[R%d]" C_RESET " %-2d  ", st - 1, i + 1);
            }

            if ((i + 1) % 5 == 0) printf("\n");
        }
        printf("\n [1-20] Gest. | [V] Vaciar | [0] Volver\n Opcion: ");
        fgets(inputBuffer, sizeof(inputBuffer), stdin);

        if (toupper(inputBuffer[0]) == 'V') {
            printf(C_ROJO " Seguro? (S/N): " C_RESET);
            char conf; scanf_s(" %c", &conf, 1); getchar();
            if (toupper(conf) == 'S') {
                for (int k = 0; k < FIGURAS_POR_EQUIPO; k++) eq->figuras[k].estado = 0;
                guardarAlbum();
                registrarMovimiento(eq->codigo, 0, "VACIADO DE EQUIPO");
            }
            continue;
        }

        int f_idx = atoi(inputBuffer) - 1;
        if (inputBuffer[0] == '0') break;
        if (f_idx < 0 || f_idx >= FIGURAS_POR_EQUIPO) continue;

        printf("\n [1] Obtener/Sumar | [2] Eliminar | [0] Volver\n Opcion: ");
        int opcion = leerEntero();
        if (opcion == 1) {
            eq->figuras[f_idx].estado++;
            guardarAlbum();
            registrarMovimiento(eq->codigo, f_idx + 1, "SUMADA");
        }
        else if (opcion == 2 && eq->figuras[f_idx].estado > 0) {
            eq->figuras[f_idx].estado--;
            guardarAlbum();
            registrarMovimiento(eq->codigo, f_idx + 1, "ELIMINADA");
        }
    }
}

int main() {
    cargarAlbum();

    while (1) {
        limpiarPantalla();
        mostrarResumen();
        mostrarEstadisticas();

        printf("\n" C_CIAN "--- MENU PRINCIPAL ---" C_RESET "\n");
        printf(" [A-L] Seleccionar Grupo\n [W]   Institucionales (FWC)\n [4]   Borrar Album\n [5]   Carga Manual\n [6]   Ver Historial\n [7]   Borrar Historial\n [0]   Salir\n Opcion: ");

        char inputBuffer[10];
        fgets(inputBuffer, sizeof(inputBuffer), stdin);

        inputBuffer[strcspn(inputBuffer, "\n")] = 0;

        if (strlen(inputBuffer) != 1) {
            continue;
        }

        char input = toupper(inputBuffer[0]);

        if (input == '0') break;

        if (input == '4') {
            printf(C_ROJO " Seguro? (S/N): " C_RESET);
            char conf; scanf_s(" %c", &conf, 1); getchar();
            if (toupper(conf) == 'S') {
                inicializarAlbum();
                guardarAlbum();
                registrarMovimiento("TODO", 0, "ALBUM BORRADO COMPLETO");
            }
            continue;
        }

        if (input == '5') { cargarPorCodigo(); guardarAlbum(); continue; }

        if (input == '6') {
            FILE* file;
            if (fopen_s(&file, "historial.log", "r") == 0) {
                fclose(file);
                system("notepad historial.log");
            }
            else {
                printf(C_ROJO "\n >> El historial esta vacio o no existe aun." C_RESET "\n");
                system("pause");
            }
            continue;
        }

        // --- NUEVA OPCION PARA BORRAR EL LOG ---
        if (input == '7') {
            printf(C_ROJO " Seguro que deseas borrar el historial de movimientos? (S/N): " C_RESET);
            char conf; scanf_s(" %c", &conf, 1); getchar();
            if (toupper(conf) == 'S') {
                if (remove("historial.log") == 0) {
                    printf(C_VERDE "\n >> Historial borrado con exito." C_RESET "\n");
                }
                else {
                    printf(C_ROJO "\n >> No se pudo borrar el historial o no existe." C_RESET "\n");
                }
                system("pause");
            }
            continue;
        }

        if (input == 'W') {
            gestionarEquipoMenu(&fwc);
            continue;
        }

        int g_idx = -1;
        for (int i = 0; i < NUM_GRUPOS; i++) if (album[i].letra == input) g_idx = i;
        if (g_idx == -1) continue;

        printf(" Equipo (1-4): ");
        int e_idx = leerEntero() - 1;
        if (e_idx < 0 || e_idx >= EQUIPOS_POR_GRUPO) continue;

        gestionarEquipoMenu(&album[g_idx].equipos[e_idx]);
    }
    return 0;
}