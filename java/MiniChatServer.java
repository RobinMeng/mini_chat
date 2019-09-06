import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class MiniChatServer {

    private static final int port = 6666;

    private static List<Socket> clientSockets = new ArrayList<>();

    private static void initServer() throws IOException {
        System.out.println("init mini_chat server....");
        ServerSocket serverSocket = new ServerSocket(port);

        while (true) {
            Socket socket = serverSocket.accept();
            new Thread(new Runnable() {
                @Override
                public void run() {
                    transfer(socket);
                }
            }).start();
            clientSockets.add(socket);
        }
    }

    private static void transfer(Socket currentSocket) {
        while (true) {
            try {
                byte[] bytes = new byte[1024];
                InputStream inputStream = currentSocket.getInputStream();
                inputStream.read(bytes);
                for (Socket socket : clientSockets) {
                    if (socket == currentSocket) {
                        continue;
                    }
                    System.out.println("recv message from client:" + socket.getRemoteSocketAddress().toString() + " message:" + new String(bytes, 0, bytes.length, "UTF-8"));
                    socket.getOutputStream().write(bytes);
                }
            } catch (Exception e) {
                throw new RuntimeException();
            }

        }

    }

    public static void main(String[] args) {

        try {
            initServer();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
