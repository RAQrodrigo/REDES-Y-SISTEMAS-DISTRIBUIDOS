from flask import Flask, jsonify, request
import proximo_feriado_api as proximo_feriado
import random

app = Flask(__name__)

peliculas = [
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


def obtener_peliculas():
    return jsonify(peliculas)


def obtener_pelicula(id):
    pelicula_encontrada = {}
    for pelicula in peliculas:
        if pelicula['id'] == id:
            pelicula_encontrada = pelicula
            break
    return jsonify(pelicula_encontrada)


def agregar_pelicula():
    nueva_pelicula = {
        'id': obtener_nuevo_id(),
        'titulo': request.json['titulo'],
        'genero': request.json['genero']
    }
    peliculas.append(nueva_pelicula)
    print(peliculas)
    return jsonify(nueva_pelicula), 201


def actualizar_pelicula(id):
    pelicula_actualizada = {}
    for pelicula in peliculas:
        if pelicula['id'] == id:
            pelicula['titulo'] = request.json['titulo']
            pelicula['genero'] = request.json['genero']
            pelicula_actualizada = pelicula
            break
    return jsonify(pelicula_actualizada), 200


def eliminar_pelicula(id):
    for pelicula in peliculas:
        if pelicula['id'] == id:
            peliculas.remove(pelicula)
            break
    for pelicula in peliculas:
        if pelicula['id'] > id:
            pelicula['id'] -= 1

    return jsonify({'mensaje': 'Película eliminada correctamente'}), 200


def obtener_nuevo_id():
    if len(peliculas) > 0:
        ultimo_id = peliculas[-1]['id']
        return ultimo_id + 1
    else:
        return 1


def obtener_peliculas_por_genero(genero):
    genero = genero.replace("_", " ")
    genero = genero.encode('latin1').decode('utf-8')
    peliculas_por_genero = []
    for pelicula in peliculas:
        if pelicula['genero'] == genero:
            peliculas_por_genero.append(pelicula)
    if peliculas_por_genero:
        return jsonify(peliculas_por_genero), 200
    else:
        return jsonify({'error No hay peliculas del genero': genero}), 404


def obtener_p_aleatoria():
    p_aleatoria = random.choice(peliculas)
    return jsonify(p_aleatoria), 200


def obtener_p_aleatoria_por_genero(genero):
    genero = genero.replace("_", " ")
    genero = genero.encode('latin1').decode('utf-8')
    peliculas_del_genero = [pelicula for pelicula in peliculas if pelicula['genero'] == genero]
    if peliculas_del_genero:
        p_aleatoria = random.choice(peliculas_del_genero)
        return jsonify(p_aleatoria), 200
    else:
        return jsonify({'error: No hay peliculas del genero': genero}), 404


def obtener_peliculas_por_titulo(caracteres):
    caracteres = caracteres.replace("_", " ")
    caracteres = caracteres.encode('latin1').decode('utf-8')
    caracteres_en_minusculas = caracteres.lower()
    peliculas_por_caracteres = []
    for pelicula in peliculas:
        titulo = pelicula['titulo'].lower()
        if caracteres_en_minusculas in titulo:
            peliculas_por_caracteres.append(pelicula)
    return jsonify(peliculas_por_caracteres)


def obtener_peli_prox_feriado(genero):
    genero = genero.replace("_", " ")
    genero = genero.encode('latin1').decode('utf-8')
    next_holiday = proximo_feriado.NextHoliday()
    next_holiday.fetch_holidays()
    lista_peliculas = [pelicula for pelicula in peliculas if pelicula['genero'] == genero]
    if not lista_peliculas:
        return jsonify({'error: No hay peliculas del genero': genero}), 404
    peli_aleatoria = random.choice(lista_peliculas)
    data_holiday = {'reason': next_holiday.holiday['motivo'], 'day': next_holiday.holiday['dia'],
                    'month': next_holiday.holiday['mes'], 'year': next_holiday.year}
    return jsonify({'next_holiday': data_holiday, 'random_movie': peli_aleatoria}), 200


app.add_url_rule('/peliculas', 'obtener_peliculas', obtener_peliculas, methods=['GET'])
app.add_url_rule('/peliculas/<int:id>', 'obtener_pelicula', obtener_pelicula, methods=['GET'])
app.add_url_rule('/peliculas/<string:genero>', 'obtener_peliculas_por_genero', obtener_peliculas_por_genero,
                 methods=['GET'])
app.add_url_rule('/peliculas/titulo/<string:caracteres>', 'obtener_peliculas_por_titulo',
                 obtener_peliculas_por_titulo, methods=['GET'])
app.add_url_rule('/peliculas', 'agregar_pelicula', agregar_pelicula, methods=['POST'])
app.add_url_rule('/peliculas/<int:id>', 'actualizar_pelicula', actualizar_pelicula, methods=['PUT'])
app.add_url_rule('/peliculas/<int:id>', 'eliminar_pelicula', eliminar_pelicula, methods=['DELETE'])
app.add_url_rule('/peliculas/random', 'obtener_p_aleatoria', obtener_p_aleatoria, methods=['GET'])
app.add_url_rule('/peliculas/random/<string:genero>', 'obtener_p_aleatoria_por_genero',
                 obtener_p_aleatoria_por_genero, methods=['GET'])
app.add_url_rule('/peliculas/random/<string:genero>/next_holiday', 'obtener_peli_prox_feriado',
                 obtener_peli_prox_feriado, methods=['GET'])

if __name__ == '__main__':
    app.run()
