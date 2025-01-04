# Documentación de la API

## Introducción
La API es esencialmente una base de datos de peliculas , donde a la misma se le pueden hacer distintas consultas, entre ellas buscar una pelicula por genero. A la cual le agregamos algunas funciones , como que me recomiende una pelicula para el proximo feriado, Donde esta misma utiliza otra API para saber los proximos feriados.

Nuestro proposito es que las compañias puedan tener su base de datos de peliculas , pedir recomendacion , y eliminar o agregar segun sus necesidades. Por ende esta dirigido para empresa dedicada a distribuir filmes.

## Instalación y Configuración
Recomendamos usar un **entorno virtual** con venv:
        python3 -m venv .venv #
Activamos el entorno virtual:
        source .venv/bin/activate
e instalamos las librerias necesarias:
        pip install -r requeriments.txt

Lo siguiente seria ejecutar la API en main.py y listo.

Tenemos varias dependencias, todas estan en requeriments.txt, pero tenemos por ejemplo:
Flask,pytest,etc.

### Como usar la api

####  Obtener lista de peliculas:
        curl http://127.0.0.1:5000/peliculas

#### Obtener pelicula especifica
        curl http://127.0.0.1:5000/peliculas/<ID>

#### Obtener lista de peliculas por genero
        curl http://127.0.0.1:5000/peliculas/<genero>
        
#### Obtener pelicula para proximo feriado
        curl http://127.0.0.1:5000/peliculas/random/<genero>/next_holiday

#### Obtener pelicula random
        curl http://127.0.0.1:5000/peliculas/random

#### Obtener pelicula random por genero
        curl http://127.0.0.1:5000/peliculas/random/<genero>

#### Obtener peliculas por titulo
        curl http://127.0.0.1:5000/peliculas/titulo/<titulo>

#### Agregar pelicula
        curl -X POST http://127.0.0.1:5000/peliculas -H 'Content-Type: application/json' -d '{"titulo": "<titulo>", "genero": "<genero>"}'

#### Actualizar pelicula
        curl -X PUT http://127.0.0.1:5000/peliculas/<ID> -H 'Content-Type: application/json' -d '{"titulo": "<nuevo titulo>", "genero": "<nuevo genero>"}'

#### Eliminar pelicula
        curl -X DELETE http://127.0.0.1:5000/peliculas/<ID>

## Referencia de Endpoints

    '/peliculas' - obtener_peliculas: Este endpoint es de tipo GET y se utiliza para obtener todas las películas en la base de datos. No toma ningún parámetro adicional.
        Respuesta:
        [
                {'id': 1, 'titulo': 'Indiana Jones', 'genero': 'Acción'},
                {'id': 2, 'titulo': 'Star Wars', 'genero': 'Acción'},
                {'id': 3, 'titulo': 'Interstellar', 'genero': 'Ciencia ficción'},
                {'id': 4, 'titulo': 'Jurassic Park', 'genero': 'Aventura'},
                {'id': 5, 'titulo': 'The Avengers', 'genero': 'Acción'},
                {'id': 6, 'titulo': 'Back to the Future', 'genero': 'Ciencia ficción'},
                {'id': 7, 'titulo': 'The Lord of the Rings', 'genero': 'Fantasía'},
                {'id': 8, 'titulo': 'The Dark Knight', 'genero': 'Acción'},
                {'id': 9, 'titulo': 'Inception', 'genero': 'Ciencia ficción'},
                {'id': 10, 'titulo': 'The Shawshank Redemption', 'genero': 'Drama'},
                {'id': 11, 'titulo': 'Pulp Fiction', 'genero': 'Crimen'},
                {'id': 12, 'titulo': 'Fight Club', 'genero': 'Drama'}
        ]

    '/peliculas/int:id' - obtener_pelicula: Este endpoint es de tipo GET y se utiliza para obtener una película en base a su ID. Toma un parámetro en la URL que es el ID de la película que se desea obtener.

    -Solicitud-:
        /peliculas/3
    -Respuesta-:
        {'id': 3, 'titulo': 'Interstellar', 'genero': 'Ciencia ficción'}

    '/peliculas/string:genero' - obtener_peliculas_por_genero: Este endpoint es de tipo GET y se utiliza para obtener películas filtradas por género. Toma un parámetro en la URL que es el género por el cual se desea filtrar las películas.

        -solicitud-:
                /peliculas/Acción
        -Respuesta-:
        [
                {'id': 1, 'titulo': 'Indiana Jones', 'genero': 'Acción'},
                {'id': 2, 'titulo': 'Star Wars', 'genero': 'Acción'},
                {'id': 5, 'titulo': 'The Avengers', 'genero': 'Acción'},
                {'id': 8, 'titulo': 'The Dark Knight', 'genero': 'Acción'}
        ]

    '/peliculas' - agregar_pelicula: Este endpoint es de tipo POST y se utiliza para agregar una nueva película a la base de datos. Toma datos de la película en el cuerpo de la solicitud HTTP.

    '/peliculas'
        Cuerpo de la solicitud:
                {'id': 13, 'titulo': 'The Matrix', 'genero': 'Ciencia ficción'}
        Respuesta:
                Película agregada correctamente.

    '/peliculas/int:id' - actualizar_pelicula: Este endpoint es de tipo PUT y se utiliza para actualizar la información de una película existente en la base de datos. Toma un parámetro en la URL que es el ID de la película que se desea actualizar, y los nuevos datos de la película en el cuerpo de la solicitud HTTP.

    '/peliculas/4'
        Cuerpo de la solicitud:
                {'titulo': 'Jurassic World', 'genero': 'Aventura'}
        Respuesta:
                Película actualizada correctamente.

    '/peliculas/int:id' - eliminar_pelicula: Este endpoint es de tipo DELETE y se utiliza para eliminar una película de la base de datos. Toma un parámetro en la URL que es el ID de la película que se desea eliminar.

    '/peliculas/9'
        Respuesta:
                Película eliminada correctamente.

    '/peliculas/random' - obtener_p_aleatoria: Este endpoint es de tipo GET y se utiliza para obtener una película aleatoria de la base de datos.

    '/peliculas/random'
        Respuesta:
                {'id': 2, 'titulo': 'Star Wars', 'genero': 'Acción'}

    '/peliculas/random/string:genero' - obtener_p_aleatoria_por_genero: Este endpoint es de tipo GET y se utiliza para obtener una película aleatoria de un género específico. Toma un parámetro en la URL que es el género por el cual se desea filtrar las películas aleatorias.
    
    '/peliculas/random/Drama'
        Respuesta:
                {'id': 10, 'titulo': 'The Shawshank Redemption', 'genero': 'Drama'}

    '/peliculas/random/string:genero/next_holiday' - obtener_peli_prox_feriado: Este endpoint es de tipo GET y se utiliza para obtener una película aleatoria de un género específico, pero relacionada con el próximo feriado. Toma un parámetro en la URL que es el género por el cual se desea filtrar las películas aleatorias, y proporciona la película relacionada con el próximo feriado.

    '/peliculas/random/Drama/next_holiday'
        Respuesta:
                {'id': 12, 'titulo': 'Fight Club', 'genero': 'Drama'}

## Consideraciones de Seguridad
Utilizamos encode para poder traducir los nombres con tildes, o caracteres especiales en general
 