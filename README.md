# Gestor de Álbum de Figuritas (C)

Este programa es una herramienta de terminal desarrollada en C para gestionar el inventario y progreso de un álbum de figuritas. Permite llevar un control detallado de las figuritas obtenidas, repetidas, y generar estadísticas de completado.

## Características Principales

* **Gestión por Grupos:** Estructura organizada de la A a la L, con soporte para 4 equipos por grupo y 20 figuritas por equipo.
* **Sección Institucional (FWC):** Módulo dedicado para figuritas especiales (FWC), con su propia gestión y conteo.
* **Interfaz Visual Dinámica:** Uso de códigos ANSI para colorear la consola (ej. verde para figuritas OK, rojo para faltantes, azul para repetidas).
* **Estadísticas en Tiempo Real:** Cálculo automático del porcentaje de progreso tanto a nivel de equipo como a nivel global del álbum.
* **Sistema de Auditoría (Logs):** Registro automático de cada movimiento realizado (sumas, eliminaciones, vaciados) con fecha y hora exacta en `historial.log`.
* **Persistencia de Datos:** El progreso se guarda automáticamente en `album.dat` tras cada cambio.

## Instalación y Ejecución

1. **Requisitos:** Un compilador de C compatible con Windows (como MinGW o MSVC) para ejecutar funciones nativas como `system("cls")`, `fopen_s` y `system("notepad ...")`.
2. **Compilación:** Compila el archivo fuente utilizando tu entorno de preferencia.
   * Ejemplo: `gcc main.c -o album.exe`
3. **Ejecución:** Simplemente ejecuta el archivo generado `.exe`.

## Guía de Menú

| Tecla | Acción |
| :--- | :--- |
| **A-L** | Acceder a la gestión de figuritas de un grupo específico. |
| **W** | Gestionar figuritas Institucionales (FWC). |
| **4** | Borrar todo el álbum (requiere confirmación). |
| **5** | Carga manual rápida mediante código de equipo. |
| **6** | Ver historial de movimientos (abre el log en Bloc de Notas). |
| **7** | Borrar el historial de movimientos (requiere confirmación). |
| **0** | Salir del programa. |

## Estructura de Archivos

* `album.dat`: Archivo binario donde se guarda todo tu progreso. No modificar manualmente.
* `historial.log`: Archivo de texto plano que registra cada acción del usuario. Se actualiza automáticamente.

## Notas Técnicas

* **Optimización:** El código utiliza estructuras de datos (`struct`) para la organización y punteros para la navegación eficiente en la memoria, minimizando la redundancia en los bucles de cálculo.
* **Compatibilidad:** Diseñado específicamente para entornos Windows debido al uso de comandos de sistema nativos para la limpieza de pantalla y edición de logs.

---
## Autor
**Lucas Lasciarrea**  
Estudiante de Ingeniería en Sistemas (UAI) | Informático Técnico  

*Desarrollado para seguimiento local eficiente y alta disponibilidad de datos.*
