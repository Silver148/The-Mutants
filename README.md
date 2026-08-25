# THE MUTANTS #

Este fue un juego que había hecho con alguien de mi clase, está hecho en C con la librería SDL2

# Como compilar el juego

Para compilar el juego necesitas tener w64devkit en tu PC si usas Windows(O WSL en Windows 10-11, pero se requieren las siguientes librerías: SDL2, SDL2-Mixer y SDL2-Image, y el compilador de C(para WSL es gcc) y también la herramienta Make). En el caso que uses Linux requieres las librería SDL2, SDL2-Mixer y SDL2-Image, y también tener el compilador de C para Linux(gcc) y la herramienta Make.

## Modo DEBUG

Se puede compilar el juego en modo DEBUG. Para compilar el juego en modo DEBUG se escribe el comando:
```
make debug-windows-x64
```

# Requisitos minimos y recomendados para correr The Mutants

| Especificación | Requisitos Mínimos| Recomendado|
| :--- | :--- | :--- |
| **Sistema Operativo** | Windows 7 SP1 o superior | Windows 10 / 11 |
| **Procesador** | Intel Pentium 4 / Athlon 64 (Soporte SSE2 obligatorio) | Intel Core 2 Duo / i3 o superior |
| **Memoria RAM** | 1 GB de RAM | 2 GB de RAM o más |
| **Gráficos** | Intel GMA 3100 | Gráficos dedicados o gráficos integrados Intel HD Graphics/AMD Radeon  |
| **Almacenamiento** | 125 MB de espacio disponible | 150 MB de espacio disponible |
| **Notas Adicionales** | Requiere soporte de instrucciones **SSE2** 

## Cosas que se llegaron a agregar

* Menú

* IA de zombies

* Niveles (El compañero terminó siendo lo peor que le pudo suceder a mi 1er juego sin usar motor gráfico)

* Oleadas de zombies

* Sistema de manejo de armas para el jugador (algo básico)

* Sistema de vida en el jugador

# CREDITOS(Y la verdad de todo esto)

Desarrolladores: Juan Yaguaro(aka silverhacker), el otro solo contaminó esto con vibe coding :v(no menciono nombre real para evitar líos legales y porque merece anonimato total en este lugar), quedará este juego como parte de un aprendizaje algo amargo.

Originalmente este juego era compatible con Linux(ya que me había movido a Ubuntu por un tiempo para probar), luego quité la compatibilidad porque cambié a Windows(sin dual boot), pero pensé en volverla a implementar, pero por el final que tuvo este proyecto no pude cumplir eso que me propuse. Aunque este fue de los mayores proyectos que hice en lenguaje C, la verdad fue muy difícil lidiar con el compañero con el que me puse a hacer este juego, ya que eramos amigos. Ocurrieron muchos choques creativos, algunas discusiones porque este es pésimo con la programación en C++ y C, así que muchas veces lo ayudé, lo cual fue un grave error que cometí, porque prácticamente hice el juego yo solo, y él contaminó todo con código hecho con IA que era propenso a buguearse. Fui paciente, pero por problemas personales con él decidí no seguir con este juego, así que comencé otro proyecto, el cual es el que sigo desarrollando y mejorando(es una asistente virtual hecha en C++ llamada [Ada](https://github.com/Silver148/Ada-Virtual-Assistant)). La verdad The Mutants fue parte de mi aprendizaje, y me enseñó que no puedo ser ingenuo y trabajar con cualquiera, ya que hay gente incompetente que fingen saber mucho y solo empeoran todo, pero bueno, al menos estoy tranquilo ahora sabiendo que estoy haciendo un proyecto que cualquiera puede aportar y con la licencia más permisiva que hay :D, algo que realmente no es efímero y que fue hecho para que sea útil en tu vida ;).
