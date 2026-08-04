
**--------------------------------------------------------------**

**Simulador de Álbum de Figuritas**

**Autor:** Lucas Lasciarrea  
**Institución:** Universidad Abierta Interamericana (UAI)  
**Carrera:** Ingeniería en Sistemas Informáticos  
**Materia:** Introducción a los Algoritmos y la Programación  
**Año:** 2026  


**--------------------------------------------------------------**

## 1. ¿Qué es el programa?

Este programa es un simulador interactivo de un álbum de figuritas escrito íntegramente en C. Permite gestionar una colección completa de equipos, simulando la experiencia real de abrir sobres, conseguir figuritas nuevas y acumular repetidas.

El sistema cuenta con persistencia de datos (guarda el progreso en un archivo binario para no perder la colección al cerrar el programa) y un registro (log) de todos los movimientos realizados. Entre sus funciones principales se destacan:

*   Gestión y visualización de grupos y equipos.
*   Apertura simulada de sobres con animaciones de consola.
*   Estadísticas de completado en tiempo real.
*   Ranking de los equipos más completos (ordenados algorítmicamente).
*   Carga manual de figuritas mediante códigos de equipo.

---

## 2. ¿Cómo se compila?

Este código hace uso de librerías nativas de Windows (`<windows.h>`) para las animaciones y funciones de cadena/archivos seguras estándar de Microsoft (como `fopen_s` y `strcpy_s`).

**Entorno recomendado:** Visual Studio (MSVC)

### Pasos para compilar en Visual Studio:
1. Abrir proyecto solución (.sln) desde Visual Studio 2022.
2. Acceder al proyecto "FiguriTracks" desde el explorador de soluciones.
3. Acceder a la carpeta: "Archivos de Origen".
4. Entrar al archivo main.cpp.
5. Correr el codigo normalmente (Ctrl + F5).

> **Nota para usuarios de GCC/MinGW:** Si se desea compilar por consola usando GCC, es posible que el compilador no reconozca `fopen_s` o `strcpy_s` de forma predeterminada sin las flags adecuadas, ya que son extensiones seguras (C11 bounds-checking interfaces).

---

## 3. ¿Cómo se usa?

Al ejecutar el programa, se mostrará un resumen general del álbum y las estadísticas actuales. Debajo aparecerá el Menú Principal, el cual se controla ingresando letras o números según la opción deseada y presionando `ENTER`.

### Opciones del Menú Principal:
*   **[A-L] Seleccionar Grupo:** Permite entrar a un grupo específico para ver sus equipos y luego ingresar a uno para gestionar sus figuritas individualmente (sumar, restar, o vaciar el equipo).
*   **[W] Institucionales:** Acceso directo a las figuritas especiales (FWC).
*   **[4] Borrar Álbum:** Reinicia por completo el progreso de la colección.
*   **[5] Carga Manual:** Permite ingresar el código de un equipo (ej. "ARG") y el número de la figurita (1-20) para cargarla directamente.
*   **[6] Ver Historial:** Abre el bloc de notas de Windows para mostrar el archivo `historial.log` con el registro de todo lo que pasó en la sesión.
*   **[7] Borrar Historial:** Limpia el archivo log de movimientos.
*   **[8] Ver Ranking:** Muestra el Top 10 de los equipos con mayor porcentaje de completado, ordenados de mayor a menor.
*   **[9] Abrir un Sobre:** Simula la apertura de un sobre de 5 figuritas, indicando cuáles son nuevas y cuáles repetidas con indicadores de color.
*   **[0] Salir:** Cierra el programa de forma segura.

### Archivos que acompañan al main:
*   **album.dat:** Archivo binario creado automáticamente donde se guarda el progreso del usuario.
*   **historial.log:** Archivo de texto que registra fecha, hora y acción de cada modificación en el álbum.
