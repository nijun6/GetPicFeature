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
			ServerSocket ss = new ServerSocket(5099); // ����ServerSocket
			Socket s = ss.accept(); // �ȴ����գ����յ�����ʱ������Ը������Socket�����յ�����ǰ��һֱ����
			InputStream ips = s.getInputStream(); // ����������
			OutputStream ops = s.getOutputStream(); // ���������
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
