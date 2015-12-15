import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

import org.apache.lucene.document.BinaryDocValuesField;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.TextField;
import org.apache.lucene.util.BytesRef;

public class DumpFea {
	
	public void dump(String idfile, String feafile) throws IOException, WrongFormatFeaString {
		byte[] b = getFeaFromFile(feafile);
		String fea = b.toString();
		if (!fea.startsWith("filename:")) {
			throw new WrongFormatFeaString();
		}
		String filename = fea.substring("filename:".length(), fea.indexOf('\n'));
		b = getBytesFromFile(idfile, 128);
		dump(b, filename, fea);
	}
	
	public void dump(byte[] ID, String filename, String fea) throws WrongFormatFeaString {
		Document doc = new Document();
		doc.add(new BinaryDocValuesField("ID", new BytesRef(ID)));
		doc.add(new Field("filename", filename, TextField.TYPE_STORED));
		String[] fea_content = fea.split("\n");
		HashMap<String, String> fea2content = new HashMap<String, String>();
		for (String s: fea_content) {
			String[] ss = s.split(":");
			if (ss.length != 2 || !PicFeature.checkInFeaNames(ss[0])) {
				throw new WrongFormatFeaString();
			}
			fea2content.put(ss[0], ss[1]);
		}
	}
	
	private byte[] getBytesFromFile(String file, int len) throws IOException {
		InputStream in = new FileInputStream(new File(file));
		int size = in.available();
		if (size < len) {
			in.close();
			return null;
		}
		byte[] b = new byte[len];
		in.read(b, 0, len);
		in.close();
		return b;
	}	
	
	private byte[] getFeaFromFile(String file) throws IOException {
		InputStream in = new FileInputStream(new File(file));
		int size = in.available();
		byte[] b = new byte[size];
		in.read(b, 0, size);
		in.close();
		return b;
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
