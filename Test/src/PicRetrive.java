import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class PicRetrive {
	public int add(int a, int b) {
		return a + b;
	}

	public int add() {
		return 0;
	}

	public static void main(String[] args) {
		try {
			ServerSocket ss = new ServerSocket(5099); // 创建ServerSocket
			Socket s = ss.accept(); // 等待接收，接收到请求时创建针对该请求的Socket，接收到请求前会一直阻塞
			InputStream ips = s.getInputStream(); // 输入流对象
			OutputStream ops = s.getOutputStream(); // 输出流对象
			ops.write("Hello nijsdifjasdf  world!".getBytes());
			byte[] buf = new byte[1024];
			/*
			 * int len=ips.read(buf); //read(); System.out.print(new
			 * String(buf,0,len));
			 */
			String str = "x";
			while (!(str.equals("over"))) {
				BufferedReader br = new BufferedReader(new InputStreamReader(ips));
				str = (br.readLine()).toString(); // BufferedReader.readLine()
				System.out.println(str);

			}
			ips.close();
			ops.close();
			s.close();
			ss.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
