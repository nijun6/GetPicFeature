import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.queryparser.flexible.standard.config.NumericFieldConfigListener;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

public class test {
	public static void main(String[] args) throws IOException, ParseException, WrongFormatFeaString {
		Search();
	}
	
	public static void checkReader() throws IOException {
		Directory directory = FSDirectory.open(Paths.get("D:\\NJ\\prg\\picFea\\picFea\\picLib\\index"));
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		IndexReader reader = searcher.getIndexReader();
		ArrayList<String> arrayList = new ArrayList<String>();
		
		for (int i = 0; i < reader.maxDoc(); i++) {
			arrayList.add(searcher.doc(i).get("ID"));
		}
		arrayList.sort(new Comparator<String>() {
			@Override
			public int compare(String arg0, String arg1) {
				return arg0.compareTo(arg1);
			}
		});
		
		for (int i = 0; i < reader.maxDoc(); i++) {
			System.out.println("i = " + i + " " + arrayList.get(i));
		}
	}
	
	public static void Search() throws IOException, ParseException {
		Directory directory = FSDirectory.open(Paths.get("D:\\NJ\\prg\\picFea\\picFea\\picLib\\index"));
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		IndexReader indexReader = searcher.getIndexReader();
		System.out.println("there are " + indexReader.numDocs() + " documents in the index");
		QueryParser parser = new QueryParser("ID", new StandardAnalyzer());
		String id = "104 0 -45 5 108 0 -45 5 112 0 -45 5 116 0"
				+ " -45 5 120 0 -45 5 124 0 -45 5 -128 0 -45 5 -124 0 -45 5 -120 0 -45 5 -116 0 -45"
				+ " 5 -112 0 -45 5 -108 0 -45 5 -104 0 -45 5 -100 0 -45 5 -96 0 -45 5 -92 0 -45 5"
				+ " -88 0 -45 5 -84 0 -45 5 -80 0 -45 5 -76 0 -45 5 -72 0 -45 5 -68 0 -45 5 -64 "
				+ "0 -45 5 -60 0 -45 5 -56 0 -45 5 -52 0 -45 5 -48 0 -45 5 -44 0 -45 5 -40 0"
				+ " -45 5 -36 0 -45 5 -32 0 -45 5 -28 0 -45 5";
		Query query = parser.parse(id);
		TopDocs topDocs = searcher.search(query, 10);
		for (ScoreDoc scoreDoc: topDocs.scoreDocs) {
			System.out.println(scoreDoc);
		}
	}
	
	public static void build() throws IOException, WrongFormatFeaString, ParseException {
		File idFiles = new File("D:\\NJ\\prg\\picFea\\picFea\\picLib\\ID");
		File feaFiles = new File("D:\\NJ\\prg\\picFea\\picFea\\picLib\\fea");
		FeaLib feaLib = new FeaLib("D:\\NJ\\prg\\picFea\\picFea\\picLib\\index");
		for (int i = 0; i < idFiles.listFiles().length; i++) {
			PicFeature picFeature = new PicFeature(idFiles.listFiles()[i].getPath()
					, feaFiles.listFiles()[i].getPath());
			feaLib.addFeature(picFeature);
		}		
	}
}
