# Pacman
¡Bienvenido! Este es el proyecto Pacman, donde se recrea un nivel del clásico juego de Namco. Éste lo realicé basándome en los videos de deividcoptero, los cuales tomé como inicio de mi aprendizaje en programación de videojuegos, tema que siempre me ha gustado. Si bien está lejos del juego original, tanto en calidad técnica como jugabilidad, me sirvió para entender muchas técnicas de esta disciplina ¡Espero que sea de tu agrado!

## Tecnologías utilizadas
El código está hecho en C++ y se utiliza la librería Allegro en su versión 4.2.2

## Cómo ejecutar el juego
El proyecto incluye lo necesario para que funcione simplemente ejecutando pacman.exe

## Cómo jugar
Se puede mover el personaje con las teclas WASD. El objetivo es que pacman coma todas las bolitas sin ser atrapado por los fantasmas

**Consejo:** 
>Para que la jugabilidad sea mejor y el personaje responda como es debido, se recomienda no presionar y retirar inmediatamente cada tecla, sino mantenerla presionada por al menos un segundo cuando se cambia el sentido del personaje.

## Abrir el proyecto y compilar
Para poder editar el proyecto como desarrollador, es necesario instalar Allegro 4 y configurar en el IDE las rutas necesarias para que el compilador encuentre las librerías necesarias.

### Instalando Allegro
En mi caso, utilicé la versión 4.2.2 para MinGW. Se puede descargar de [este enlace](https://sourceforge.net/projects/alleg/files/allegro-bin/4.2.2/allegro-mingw-4.2.2.zip/download). Luego:

1. Copiar los DLL de la carpeta bin a la carpeta Windows del equipo.
2. Copiar el contenido de la carpeta include a la carpeta MinGW/include.
3. Copiar el contenido de la carpeta lib a la carpeta MinGW/lib.

Por último, resta configurar la ruta en el IDE para que encuentre las librerías.