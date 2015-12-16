import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.apache.lucene.queryparser.classic.ParseException;

public class Main {

	public static void main(String[] args) throws IOException
					, WrongFormatFeaString
					, InterruptedException
					, ParseException, WrongFomatArgs {
		long begin = System.currentTimeMillis();
		System.out.println(fib(45) + " time consumed:" + (System.currentTimeMillis() - begin));
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
		} else if (args.length == 2 && args[0].equals("search")) {
			FeaLib feaLib = new FeaLib();
			if (args[1].startsWith("featurefile=")) {
				PicFeature pf = new PicFeature(args[1].split("=")[1]);
				SearchRes sRes = feaLib.search(pf);
				saveFea(sRes, "ID", args[1].split("=")[1]);
			}
		}
	}
	
	private static void saveFea(SearchRes searchRes, String idfile, String feafile) 
			throws IOException {
		OutputStream out = new FileOutputStream(new File(idfile));
		out.write(searchRes.getID(), 0, 128);
		out.close();
		out = new FileOutputStream(new File(feafile));
		byte[] bs = Double.toString(searchRes.getDistance()).getBytes();
		out.write(bs, 0, bs.length);
		out.close();
	}
	
	public static long fib(int n) {
		if (n <= 2)
			return 1;
		return fib(n-1) + fib(n-2);
	}
}
