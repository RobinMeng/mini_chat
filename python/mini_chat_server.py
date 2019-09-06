import socket
import thread

cli_fds = []


def init_socket_server():
    ip = "127.0.0.1"
    port = 6666
    server_socket = socket.socket()

    server_socket.bind((ip, port))

    server_socket.listen(100)

    while True:
        cli_fd, add = server_socket.accept()

        print("welcome client:" + str(cli_fd.getsockname()))

        cli_fds.append(cli_fd)

        thread.start_new_thread(recv_message, (cli_fd,))


def recv_message(cli_fd):
    if None == cli_fd:
        ex = Exception("current_fd is NULL")
        raise ex

    while True:
        msg = cli_fd.recv(1024)
        print("recv from client data:%s\n", msg)
        if len(msg) == 0:
            continue

        for fd in cli_fds:
            if fd == cli_fd:
                continue
            fd.send(msg)



if __name__ == '__main__':
    print("mini_chat server init ...")
    init_socket_server()
