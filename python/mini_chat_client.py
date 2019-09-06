import socket
import thread


def recv_message(current_sfd):
    while True:
        msg = current_sfd.recv(1024)
        if len(msg)!=0:
            print(msg)


def init_client():
    print ("please input nickname..")
    nick_name = input()

    ip = "127.0.0.1"
    port = 6666

    s_fd = socket.socket()
    s_fd.connect((ip, port))

    thread.start_new_thread(recv_message, (s_fd,))

    while True:
        print ("please input ...")
        msg = input()
        s_fd.send((str(nick_name) + str(" say:")) + str(msg))



if __name__ == '__main__':
    init_client()
