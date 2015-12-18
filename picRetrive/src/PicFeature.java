import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.TextField;


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
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(feafile)));
		String line;
		StringBuffer fea = new StringBuffer();
		ArrayList<String> feas = new ArrayList<String>();
		while ((line = reader.readLine()) != null) {
			feas.add(line);
		}
		for (int i = 0; i < feas.size(); i++) {
			fea.append(feas.get(i));
			if (i < feas.size() - 1)
				fea.append('\n');
		}
		reader.close();
		Init(null, fea.toString());
	}
	
	public PicFeature(Document doc) {
		this.ID = doc.getBinaryValue("ID").bytes;

		feaContent.put(filename, doc.get(filename));
		feaContent.put(wordcode, doc.get(wordcode));
		feaContent.put(sentencecode, doc.get(sentencecode));
		feaContent.put(linecode, doc.get(linecode));
		feaContent.put(project_feature, doc.get(project_feature));
	}
	
	public PicFeature(byte[] ID, String filename, String wordcode, String sentencecode
			, String linecode, String projec_feature) {
		feaContent = new HashMap<String, String>();
		this.ID = ID;
		feaContent.put(PicFeature.filename, filename);
		feaContent.put(PicFeature.wordcode, wordcode);
		feaContent.put(PicFeature.sentencecode, sentencecode);
		feaContent.put(PicFeature.linecode, linecode);
		feaContent.put(PicFeature.project_feature, projec_feature);
	}
	
	public PicFeature(String idfile, String feafile) throws IOException, WrongFormatFeaString {
		byte[] id = getBytesFromFile(idfile, 128);
		BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(feafile)));
		String line;
		StringBuffer fea = new StringBuffer();
		ArrayList<String> feas = new ArrayList<String>();
		while ((line = reader.readLine()) != null) {
			feas.add(line);
		}
		for (int i = 0; i < feas.size(); i++) {
			fea.append(feas.get(i));
			if (i < feas.size() - 1)
				fea.append('\n');
		}
		reader.close();
		System.out.println("before Init:" + fea);
		Init(id, fea.toString());
	}

	public double computeDistance(PicFeature pf, double mindis) {
		return computeProjectionDistance(feaContent.get(project_feature)
				, pf.getProjecFea(), mindis);
	}
	
	private double computeProjectionDistance(String x, String y, double mindis) {
		String[] a = x.split(" ");
		String[] b = y.split(" ");
		if (a.length > b.length) {
			String[] t = a;
			a = b;
			b = t;
		}
		double dis = 0;
		int cnt = 0;
		for (int i = 0; i < b.length - a.length + 3; i++) {
			for (int j = 0; j < a.length; j++) {
				if (i+j > b.length)
					break;
				double dis_line = dispj(a[j], b[i+j], mindis, 0.2);
				int len = a[j].length() < b[i+j].length() ? a[j].length() : b[i+j].length();
				dis += (dis*cnt + dis_line*len)/(cnt + len);
				cnt += len;
				
				if (dis < mindis)
					break;
			}
		}
		return dis;
	}
	
	public double dispj(String x, String y, double mindis, double mincmp) {
		char[] xc = x.toCharArray();
		char[] yc = y.toCharArray();
		if (xc.length > yc.length) {
			char[] t = xc;
			xc = yc;
			yc = t;
		}
		
		double dis = 0;
		for (int i = 0; i < yc.length - xc.length + 3; i++) {
			for (int j = 0; j < xc.length && i+j < yc.length; j++) {
				double tdis = 1.0*(yc[i+j]-'a')/(xc[j]-'a');
				if (tdis > 1.0)
					tdis = 1.0/tdis;
				dis = (dis*i + tdis) / (i+1);
				if (j > xc.length*mincmp && dis < mindis)
					return dis;
			}
		}
		return dis;
	}
	
	public PicFeature(byte[] id, String fea) throws WrongFormatFeaString {
		Init(id, fea);
	}
	
	private void Init(byte[] id, String fea) throws WrongFormatFeaString {
		ID = id;
		feaContent = new HashMap<String, String>();
		System.out.println(fea);
		String[] fs = fea.split("\n");
		for (String s : fs) {
			String[] ss = s.split(":");
			if (!checkInFeaNames(ss[0]))
				throw new WrongFormatFeaString();
			feaContent.put(ss[0], s.substring(s.indexOf(':')+1));
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
	
	public Document getDocument() {
		Document doc = new Document();
		doc.add(new Field("ID", getID(), TextField.TYPE_STORED));
		doc.add(new Field(filename, getFileName(), TextField.TYPE_STORED));
		doc.add(new Field(wordcode, getWordCode(), TextField.TYPE_STORED));
		doc.add(new Field(linecode, getLineCode(), TextField.TYPE_STORED));
		doc.add(new Field(sentencecode, getSentenceCode(), TextField.TYPE_STORED));
		doc.add(new Field(project_feature, getProjecFea(), TextField.TYPE_STORED));
		return doc;
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

	public String getID() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < ID.length; i++) {
			sb.append(i);
			if (i < ID.length - 1)
				sb.append(" ");
		}
		return sb.toString();
	}
}
