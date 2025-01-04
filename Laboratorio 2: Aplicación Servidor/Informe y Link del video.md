# Laboratorio 2 

### integrantes:
    - Fran Cecchi
    - Juan Gonzalez Moreno
    - Agustin Quevedo

**Link del video**: 
[Video](https://drive.google.com/file/d/1978IHLB0el4yYDcdAqSxwYBYpdxHwM2-/view?usp=drive_link)


### ¿Qué hicimos?
Hicimos una simulación de como seria una comunicacion entre servidor y cliente, donde este
último puede ver y descargar archivos.

Utilizamos sockets para la comunicación y TCP como protocolo de la misma, como tambien threads para poder atender varios clientes a la vez.

### ¿Que puede hacer nuestro servidor?
Dado que es un servidor dedicado a la __transferencia de archivos__ puede hacer cosas orientadas a esto.

Cosas como:
    
    - Imprimir en pantalla todos los archivos disponibles
    - Imprimir en pantalla el tamaño de un archivo especifico
    - Descargar un archivo especifico , solicitando un inicio y un tamaño

El servidor tiene puede atender a varios clientes a la vez usando threads, ademas de atender las request en orden.

### ¿Cómo esta organizado el código?
Dentro de **client.py** está la clase cliente junto con sus métodos, al ejecutarlo te mostrará los archivos disponibles para ser descargados.

En **connection.py** está toda la lógica de las conexiones , creamos una clase connection que tiene asignado un socket donde ocurrira la conexión. Acá tambien esta la logica de ejecución de comandos, Parseo , etc.

Ademas en **server.py** encontraremos un objeto que es el que setea las variables del socket y donde esta el loop principal para escuchar a los clientes.

### ¿Que estrategias existen para atender varios clientes a la vez? 

1. La primera estrategia es la de threads, por cada cliente tenemos un thread atendiendolo individualmente.

2. Hacer poll, esperar activamente pero utilizando una lista de sockets.

3. Enfoque basado en multiprocesos , en lugar de crear subprocesos , se crean procesos separadaos para manejar la comunicación con cada cliente.

