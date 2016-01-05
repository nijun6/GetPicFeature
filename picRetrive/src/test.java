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
		build();
	}
	
	public static void testSearch() throws IOException, WrongFormatFeaString, ParseException {
		FeaLib feaLib = new FeaLib("D:\\NJ\\prg\\picFea\\picFea\\picLib\\index");
		PicFeature picFeature = new PicFeature("D:\\NJ\\prg\\picFea\\picFea\\picLib\\D__NJ_pictures_libpic_0.png.txt");
		SearchRes sRes = feaLib.search(picFeature);
		System.out.println(sRes);
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
		String id = "104";
		Query query = parser.parse(id);
		TopDocs topDocs = searcher.search(query, 10);
		for (ScoreDoc scoreDoc: topDocs.scoreDocs) {
			System.out.println(indexReader.document(scoreDoc.doc).get("ID"));
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
