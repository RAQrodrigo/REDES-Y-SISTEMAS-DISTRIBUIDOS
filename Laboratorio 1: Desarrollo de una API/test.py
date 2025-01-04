import requests

# Obtener todas las películas
response = requests.get('http://localhost:5000/peliculas')
peliculas = response.json()
print("Películas existentes:")
for pelicula in peliculas:
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
print()

# Agregar una nueva película
nueva_pelicula = {
    'titulo': 'Pelicula de prueba',
    'genero': 'Acción'
}
response = requests.post('http://localhost:5000/peliculas', json=nueva_pelicula)
if response.status_code == 201:
    pelicula_agregada = response.json()
    print("Película agregada:")
    print(f"ID: {pelicula_agregada['id']}, Título: {pelicula_agregada['titulo']}, Género: {pelicula_agregada['genero']}")
else:
    print("Error al agregar la película.")
print()

# Obtener detalles de una película específica
id_pelicula = 1  # ID de la película a obtener
response = requests.get(f'http://localhost:5000/peliculas/{id_pelicula}')
if response.status_code == 200:
    pelicula = response.json()
    print("Detalles de la película:")
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error al obtener los detalles de la película.")
print()

# Actualizar los detalles de una película
id_pelicula = 1  # ID de la película a actualizar
datos_actualizados = {
    'titulo': 'Nuevo título',
    'genero': 'Comedia'
}
response = requests.put(f'http://localhost:5000/peliculas/{id_pelicula}', json=datos_actualizados)
if response.status_code == 200:
    pelicula_actualizada = response.json()
    print("Película actualizada:")
    print(f"ID: {pelicula_actualizada['id']}, Título: {pelicula_actualizada['titulo']}, Género: {pelicula_actualizada['genero']}")
else:
    print("Error al actualizar la película.")
print()

# Eliminar una película
id_pelicula = 1  # ID de la película a eliminar
response = requests.delete(f'http://localhost:5000/peliculas/{id_pelicula}')
if response.status_code == 200:
    print("Película eliminada correctamente.")
else:
    print("Error al eliminar la película.")

#Test para obtener una pelicula aleatoria
response = requests.get(f'http://localhost:5000/peliculas/random', json=datos_actualizados)
if response.status_code == 200:
    pelicula_aleatoria = response.json()
    print("\nPelícula aleatoria:")
    print(f"ID: {pelicula_aleatoria['id']}, Título: {pelicula_aleatoria['titulo']}, Género: {pelicula_aleatoria['genero']}")
else:
    print("Error al obtener la película aleatoria.")

#Test para obtener peliculas por genero
response = requests.get(f'http://localhost:5000/peliculas/Drama', json=datos_actualizados)
if response.status_code == 200:
    peliculas_por_genero = response.json()
    print("\nPelículas por género:")
    for pelicula in peliculas_por_genero:
        print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:    
    print("Error al obtener las películas por género.")

#Test para obtener pelicula aleatoria por genero
response = requests.get(f'http://localhost:5000/peliculas/random/Drama', json=datos_actualizados)
if response.status_code == 200:
    pelicula_aleatoria_genero = response.json()
    print("\nPelícula aleatoria por género:")
    print(f"ID: {pelicula_aleatoria_genero['id']}, Título: {pelicula_aleatoria_genero['titulo']}, Género: {pelicula_aleatoria_genero['genero']}")
else:
    print("Error al obtener la película aleatoria por género.")

#Test para obtener pelicula random para proximo feriado
response = requests.get(f'http://localhost:5000/peliculas/random/Drama/next_holiday', json=datos_actualizados)
if response.status_code == 200:
    data = response.json()
    print("\nPelícula random para próximo feriado:")
    print(f"ID: {data['random_movie']['id']}, Título: {data['random_movie']['titulo']}, Género: {data['random_movie']['genero']}")
    print(f"Dia: {data['next_holiday']['day']}, Mes: {data['next_holiday']['month']}, Año: {data['next_holiday']['year']}")
    print(f"Motivo: {data['next_holiday']['reason']}")
else:
    print("Error al obtener la data.")

#Buscar pelicula por titulo.
response = requests.get(f'http://localhost:5000/peliculas/titulo/a')
peliculas = response.json()
print("\nResultado:")
if (peliculas == []):
    print("No hay peliculas con ese titulo")
else:
    print("Peliculas existentes: ")
    for pelicula in peliculas:    
        print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
    print()