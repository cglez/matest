ACERCA DE MaTest

 MaTest (Matrix Tester) es un téster de matrices lógicas útil en la
 investigación en lógica matemática y lógica filosófica.

 Definidas unas matrices para unas conectivas, definido un mínimo valor
 designado y dada una fórmula bien formada, calcula todas las posibles
 valoraciones de dicha fórmula y evalúa en cada caso si se trata de un valor
 designado o no.

 MaTest es software libre, se distribuye bajo licencia GPLv3+, para más detalles
 véase la sección Copyright. Es multiplataforma, está programado en C estándar,
 no utiliza en su código funciones dependientes de sistema operativo; debería
 funcionar por tanto en cualquier sistema operativo para el que exista un
 compilador de C.


AUTOR

 Escrito por César González Gutiérrez <ceguel@gmail.com>.


DESCARGAS

 Puedes descargarlo desde http://forja.rediris.es/projects/cusl3-matest/
 
 La versión en desarrollo es accesible mediante Subversion en la dirección:
 https://forja.rediris.es/svn/cusl3-matest/trunk/


INSTALACIÓN

 Junto con este archivo se encuentran dos ejecutables: uno para GNU/Linux o
 similar (Matest) y otro para Windows (MaTest.exe). Basta con ejecutar el
 binario.
 
 Para compilarlo desde el código fuente puede usarse el comando make con alguno
 de los archivos Makefile preparados o editar éstos.


DOCUMENTACIÓN

 La documentación se encuentra en línea desde la siguiente dirección:
 http://www.ceguel.es/MaTest/


COPYRIGHT

 Copyright (C) 2008 César González
 Licencia GPLv3+: GPL de GNU versión 3 o posterior
 <http://gnu.org/licenses/gpl.html>
 Esto es software libre: tiene la libertad de cambiarlo y redistribuirlo.
 No tiene GARANTÍA, hasta los límites de la ley.



CHANGELOG

 MaTest-1.0 (25/11/08)
 
 *  Código fuente modular.
 
 *  Lenguaje natural del programa: cambiado al Inglés.
 
 *  Reserva dinámica de memoria.
 
 *  Dimensión de las matrices libre (limitado por la memoria disponible).
 
 *  Posibilidad de definir nuevas conectivas y de borrar las ya existentes.
 
 *  Posibilidad de guardar las matrices en un archivo externo. Éstas se guardan
    en formato de tabla simple, legible tanto por una persona como por un
    ordenador.
 
 *  Las variables son registradas por demanda.
 
 *  Menús rediseñados. Mejorada la robustez de la entrada de datos.
 
 *  La detección de las fórmulas bien formadas en notación polaca se efectúa en
    función de los datos proporcionados por el usuario según la siguiente con-
    vención: cualquier letra mayúscula es una conectiva, cualquier letra minús-
    cula es una variable.
    Mejoras en la detección de errores en fórmulas mal formadas.
 
 *  Creado un parser que transforma una fórmula bien formada en notación polaca
    en una estructura de datos tipo árbol. 
 
 *  Algoritmo de evaluación general. Las fbf individuales se evalúan mediante la
    estructura en árbol (evaluación independiente de la notación empleada),
    todas las posibilidades se calculan en función de la dimensión dada y de las
    variables definidas por el usuario (independiente de valores predefinidos).
 
 *  Posibilidad de seleccionar los valores mostrados en la evaluación, entre:
    todos, valores designados o valores no designados.
    También se muestran estadísticas después de la evaluación.


 MaTest-0.1 (First release)

 *  Archivo único de código fuente.
 
 *  Reserva estática de memoria.
 
 *  Las matrices están limitadas a una dimensión máxima de 10×10.
 
 *  Las conectivas y el mínimo valor designado son definidos automáticamente,
    después de que el usuario introduzca la dimensión de las matrices, según el
    modelo de las lógicas multivaluadas de Łukasiewicz.
 
 *  Mínimo valor designado redefinible.
 
 *  6 conectivas predefinidas: implicación, negación, conjunción, disyunción,
    necesidad y posibilidad.
 
 *  Posibilidad de redefinir las matrices introduciendo los valores uno a uno.
 
 *  5 variables: p, q, r, s, t.
 
 *  Acepta fórmulas bien formadas en notación polaca, éstas se identifican
    dependiendo de las conectivas y variables predefinidas.
 
 *  Algoritmo de evaluación implementado en base a las conectivas y variables
    predefinidas. Marca con un asterisco los valores designados.


BUGS
 
 Notificar fallos en la dirección del autor: <ceguel@gmail.com>.

