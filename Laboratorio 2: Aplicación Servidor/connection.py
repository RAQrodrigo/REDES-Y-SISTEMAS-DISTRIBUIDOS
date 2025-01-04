# encoding: utf-8
# Revisión 2019 (a Python 3 y base64): Pablo Ventura
# Copyright 2014 Carlos Bederián
# $Id: connection.py 455 2011-05-01 00:32:09Z carlos $

import socket
from constants import *
from base64 import b64encode
import os

class Connection(object):
    """
    Conexión punto a punto entre el servidor y un cliente.
    Se encarga de satisfacer los pedidos del cliente hasta
    que termina la conexión.
    """

    def __init__(self, socket, directory):
        self.socket = socket
        self.directory = directory

    def handle(self):
        """
        Atiende eventos de la conexión hasta que termina.
        """
        print(f"[CONNECTED] Connection from {self.socket.getpeername()}")
        connected = True

        while connected:
            msg = self.recv(DEFAULT_MSG_SIZE)

            msg_list = self.parse_msg(msg)

            if not msg_list:
                break

            command_ammount = self.command_ammount(msg_list)
            
            if command_ammount > 1:
                connected = self.handle_multiple_commands(msg_list)
            else:
                connected = self.handle_command(msg_list)

        self.socket.close()


    def recv(self, size):
        """
        Recibe `size' bytes del socket.
        """
        data = b''
        while True:
            chunk = self.socket.recv(size)
            data += chunk
            if data.endswith(EOL.encode(DEFAULT_FORMAT)):
                break
            
        return data.decode('utf-8')

        

    def send(self, message):
        """
        Envía un mensaje al cliente.
        """
        self.socket.send(message)

    
    def get_file_listing(self):
        """
        Lista los archivos del directorio.
        """
        try:
            files = os.listdir(self.directory)
        except OSError:
            self.send(f"{INTERNAL_ERROR} {error_messages[INTERNAL_ERROR]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[INTERNAL ERROR] from {self.socket.getpeername()}") 
            return
        
        self.send(f"{CODE_OK} {error_messages[CODE_OK]}{EOL}".encode(DEFAULT_FORMAT))
        print(f"[REQUESTED FILE LISTING] from {self.socket.getpeername()}")

        for file in files:
            self.send(f"{file}{EOL}".encode('utf-8'))

        self.send(f"{EOL}".encode('utf-8'))
        
        

    
    def get_metadata(self, filename):
        """
        Obtiene los metadatos de un archivo.
        """
        try:
            file = open(f"{self.directory}/{filename}", 'rb')
            file_size = os.path.getsize(f"{self.directory}/{filename}")
            file.close()

            self.send(f"{CODE_OK} {error_messages[CODE_OK]}{EOL}".encode(DEFAULT_FORMAT))
            self.send(f"{file_size}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[REQUESTED METADATA] for {filename} from {self.socket.getpeername()}")

        except FileNotFoundError:
            self.send(f"{FILE_NOT_FOUND} {error_messages[FILE_NOT_FOUND]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[FILE NOT FOUND] {filename} from {self.socket.getpeername()}")

        except OSError:
            self.send(f"{FILE_NOT_FOUND} {error_messages[FILE_NOT_FOUND]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[FILE NOT FOUND] {filename} from {self.socket.getpeername()}")
        

    def parse_msg(self, msg):
        """
        Parsea un mensaje.
        """
        msg = msg.strip()

        msg_list = msg.split(' ')

        eol_split_list = []

        for i in range(len(msg_list)):
            eol_split_list.extend(msg_list[i].split(EOL))
        
        for i in range(len(eol_split_list)):
            if "\n" in eol_split_list[i] or "\r" in eol_split_list[i]:
                self.send(f"{BAD_EOL} {error_messages[BAD_EOL]}{EOL}".encode(DEFAULT_FORMAT))
                print(f"[BAD EOL] from {self.socket.getpeername()}")
                return None
        
        return eol_split_list


    def command_ammount(self, msg_list):
        """
        Verifica si un mensaje contiene múltiples comandos.
        """
        i = 0
        k = 0
        for i in range(len(msg_list)):
            if msg_list[i] in VALID_COMMAND_LIST:
                k += 1

        return k
    


    def get_slice(self, filename, offset, size):
        """
        Obtiene un slice de un archivo.
        """
        try:
            file = open(f"{self.directory}/{filename}", 'rb')
            file.seek(offset)
            data = file.read(size)
            file.close()

            self.send(f"{CODE_OK} {error_messages[CODE_OK]}{EOL}".encode(DEFAULT_FORMAT))
            self.socket.send(f"{b64encode(data).decode('utf-8')}{EOL}".encode("utf-8"))
            print(f"[REQUESTED FILE SLICE] from {self.socket.getpeername()}")

        except FileNotFoundError:
            self.send(f"{FILE_NOT_FOUND} {error_messages[FILE_NOT_FOUND]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[FILE NOT FOUND] {filename} from {self.socket.getpeername()}")

        except OSError:
            self.send(f"{BAD_OFFSET} {error_messages[BAD_OFFSET]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[BAD OFFSET] from {self.socket.getpeername()}")


    def handle_command(self, msg_list):
        """
        Maneja un comando.
        """
        msg_list_len = len(msg_list)

        if msg_list[0] == 'quit' or msg_list[0] == '':
            if msg_list_len == 1:
                self.send(f"{CODE_OK} {error_messages[CODE_OK]}{EOL}".encode(DEFAULT_FORMAT))
                return False
            else:
                self.send(f"{INVALID_ARGUMENTS} {error_messages[INVALID_ARGUMENTS]}{EOL}".encode(DEFAULT_FORMAT))
                print(f"[INVALID ARGUMENTS] from {self.socket.getpeername()}")                               

        elif msg_list[0] == 'get_file_listing':
            if msg_list_len == 1:
                self.get_file_listing()
            else:
                self.send(f"{INVALID_ARGUMENTS} {error_messages[INVALID_ARGUMENTS]}{EOL}".encode(DEFAULT_FORMAT))
                print(f"[INVALID ARGUMENTS] from {self.socket.getpeername()}")              

        elif msg_list[0] == 'get_metadata':
            if msg_list_len == 2:
                self.get_metadata(msg_list[1])
            else:
                self.send(f"{INVALID_ARGUMENTS} {error_messages[INVALID_ARGUMENTS]}{EOL}".encode(DEFAULT_FORMAT))
                print(f"[INVALID ARGUMENTS] from {self.socket.getpeername()}")
        
        elif msg_list[0] == 'get_slice':
            if msg_list_len == 4:
                try:
                    offset = int(msg_list[2])
                    size = int(msg_list[3])
                    self.get_slice(msg_list[1], offset, size)
                except ValueError:
                    self.send(f"{INVALID_ARGUMENTS} {error_messages[INVALID_ARGUMENTS]}{EOL}".encode(DEFAULT_FORMAT))
                    print(f"[INVALID ARGUMENTS] from {self.socket.getpeername()}")
            else:
                self.send(f"{INVALID_ARGUMENTS} {error_messages[INVALID_ARGUMENTS]}{EOL}".encode(DEFAULT_FORMAT))
                print(f"[INVALID ARGUMENTS] from {self.socket.getpeername()}")
        
        else:
            self.send(f"{INVALID_COMMAND} {error_messages[INVALID_COMMAND]}{EOL}".encode(DEFAULT_FORMAT))
            print(f"[INVALID COMMAND]: '{msg_list[0]}' from {self.socket.getpeername()}")
        

        return True


    def handle_multiple_commands(self, msg_list):
        """
        Maneja múltiples comandos.
        """
        ## list of list, where each list is the command with its arguments
        commands_list = []
        k = -1
        j = 0
        for i in msg_list:
            if msg_list[j] in VALID_COMMAND_LIST:
                commands_list.append([msg_list[j]])
                k += 1
            else:
                commands_list[k].append(msg_list[j])
            j += 1

        state = True
        for command in commands_list:
            state = self.handle_command(command)
            if not state:
                return False
            
        return state