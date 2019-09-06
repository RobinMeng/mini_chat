import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Scanner;

public class MiniChatClient {


    public static final String ip = "127.0.0.1";
    public static final int port = 6666;

    public static void main(String[] args) {
        try {
            initClient();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void initClient() throws IOException {
        System.out.println("please input nickname");
        Scanner nickScanner = new Scanner(System.in);
        String nickName = nickScanner.next();

        Socket socket = new Socket(ip, port);
        OutputStream outputStream = socket.getOutputStream();
        new Thread(new Runnable() {
            @Override
            public void run() {
                recvMessage(socket);
            }
        }).start();

        while (true) {
            System.out.println("please input...");
            Scanner scanner = new Scanner(System.in);
            String message = nickName + ":" + scanner.next();
            outputStream.write(message.getBytes());
            outputStream.flush();
        }
    }

    public static void recvMessage(Socket socket) {
        try {
            while (true) {
                byte[] bytes = new byte[1024];
                InputStream inputStream = socket.getInputStream();
                inputStream.read(bytes);
                System.out.println(new String(bytes));
            }
        } catch (Exception e) {
            new RuntimeException(e);
        }


    }

}
