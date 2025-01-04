import proximo_feriado_api as proximo_feriado
import sys


def main():
    arguments = sys.argv[1:]

    next_holiday = proximo_feriado.NextHoliday()
    next_holiday.fetch_holidays()

    if not arguments:
        next_holiday.render()
    else:
        if arguments[0] == '-t' and len(arguments) == 2:
            holidays = next_holiday.search_holiday_for_type(arguments[1])
            if not holidays:
                print("Error: No se encontraron feriados del tipo", arguments[1])
            else:
                i = 1
                for holiday in holidays:
                    print(str(i) + ". [ Motivo: " + holiday['motivo'])
                    print("     Fecha: " + proximo_feriado.day_of_week(holiday['dia'], holiday['mes'], next_holiday.year), holiday['dia'],
                          proximo_feriado.months[holiday['mes'] - 1])
                    print("     Tipo: " + holiday['tipo'] + " ]\n")
                    i += 1
        else:
            print("Error: Argumento invalido")
            print("Uso: proximo_feriado.py [-t <tipo>]")


if __name__ == "__main__":
    main()
