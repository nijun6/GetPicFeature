import java.io.IOException;

import org.apache.lucene.queryparser.classic.ParseException;

public class Main {

	public static void main(String[] args) throws IOException
					, WrongFormatFeaString
					, InterruptedException
					, ParseException {
		if (args.length == 3 && args[0].equals("dumpfea")) {
			DumpFea dumpFea = new DumpFea();
			if (args[1].startsWith("idfile=") && args[2].startsWith("featurefile=")) {
				dumpFea.dump(args[1].split("=")[1], args[2].split("=")[1]);
			} else if (args[2].startsWith("idfile=") && args[1].startsWith("featurefile=")) {
				dumpFea.dump(args[2].split("=")[1], args[1].split("=")[1]);
			}
		} else if (args.length == 2 && args[0].equals("search")) {
			FeaLib feaLib = new FeaLib();
			if (args[1].equals("featurefile=")) {
				PicFeature pf = new PicFeature(args[1].split("=")[1]);
				SearchRes sRes = feaLib.search(pf);
			}
		}
	}
}
