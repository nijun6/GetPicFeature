import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.apache.lucene.queryparser.classic.ParseException;

public class Main {

	public static void main(String[] args) throws IOException
					, WrongFormatFeaString
					, InterruptedException
					, ParseException, WrongFomatArgs, WraongFormatID {
		System.out.println("java picfeature lib started");
		System.out.println("args.length = " + args.length);
		for (String s: args) {
			System.out.println(s);
		}
		if (args.length == 3 && args[0].equals("dumpfea")) {
			PicFeature picFeature = null;
			if (args[1].startsWith("idfile=") && args[2].startsWith("featurefile=")) {
				picFeature = new PicFeature(args[1].split("=")[1], args[2].split("=")[1]);
			} else if (args[2].startsWith("idfile=") && args[1].startsWith("featurefile=")) {
				picFeature = new PicFeature(args[2].split("=")[1], args[1].split("=")[1]);
			} else {
				throw new WrongFomatArgs();
			}
			FeaLib feaLib = new FeaLib();
			feaLib.addFeature(picFeature);
		} else if (args.length == 3 && args[0].equals("search")) {
			FeaLib feaLib;
			if (args[2].startsWith("lib="))
				feaLib = new FeaLib(args[2].split("=")[1]);
			else {
				feaLib = new FeaLib();
			}
			
			if (args[1].startsWith("featurefile=")) {
				PicFeature pf = new PicFeature(args[1].split("=")[1]);
				SearchRes sRes = feaLib.search(pf);
				saveFea(sRes, "ID", args[1].split("=")[1]);
			}
		} else if (args.length == 1 && args[0].equals("list")) {
			FeaLib feaLib = new FeaLib();
			feaLib.list();
		}
	}
	
	private static void saveFea(SearchRes searchRes, String idfile, String feafile) 
			throws IOException, WraongFormatID {
		OutputStream out = new FileOutputStream(new File(idfile));
		byte[] id = new byte[128];
		int i = 0;
		for (String s: searchRes.getID().split(" ")) {
			if (i >= 128) {
				out.close();
				throw new WraongFormatID();
			}
			id[i++] = Byte.parseByte(s);
		}
		out.write(id, 0, 128);
		out.close();
		out = new FileOutputStream(new File(feafile));
		byte[] bs = Double.toString(searchRes.getDistance()).getBytes();
		out.write(bs, 0, bs.length);
		out.close();
	}	
}
