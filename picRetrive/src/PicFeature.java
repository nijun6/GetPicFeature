import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

public class PicFeature {
	public static final String[] feaNames 
		= {"filename", "wordcode", "sentencecode", "linecode"
			, "project_feature"};
	public static final String filename = "filename"
							   , wordcode = "wordcode"
							   , sentencecode = "sentencecode"
							   , linecode = "linecode"
							   , project_feature = "project_feature";
	byte[] ID;
	HashMap<String, String> feaContent;
	
	public PicFeature(String feafile) throws WrongFormatFeaString, IOException {
		byte[] fea = getFeaFromFile(feafile);
		Init(null, fea.toString());
	}
	
	public PicFeature(String idfile, String feafile) throws IOException, WrongFormatFeaString {
		byte[] id = getBytesFromFile(idfile, 128);
		byte[] b = getFeaFromFile(feafile);
		String fea = b.toString();
		Init(id, fea);
	}

	public PicFeature(byte[] id, String fea) throws WrongFormatFeaString {
		Init(id, fea);
	}
	
	private void Init(byte[] id, String fea) throws WrongFormatFeaString {
		ID = id;
		feaContent = new HashMap<String, String>();
		String[] fs = fea.split("\n");
		for (String s : fs) {
			String[] ss = s.split(":");
			if (ss.length != 2 || !checkInFeaNames(ss[0]))
				throw new WrongFormatFeaString();
			feaContent.put(ss[0], ss[1]);
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
	
	public static boolean checkInFeaNames(String feaname) {
		for (String s: feaNames) {
			if (s.equals(feaname))
				return true;
		}
		return false;
	}
	
	public String getFileName() {
		return feaContent.get(filename);
	}
	
	public String getWordCode() {
		return feaContent.get(wordcode);
	}
	
	public String getSentenceCode() {
		return feaContent.get(sentencecode);
	}
	
	public String getLineCode() {
		return feaContent.get(linecode);
	}
	
	public String getProjecFea() {
		return feaContent.get(project_feature);
	}
}
