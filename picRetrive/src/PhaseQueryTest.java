
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.Term;
import org.apache.lucene.queryparser.classic.CharStream;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.queryparser.classic.QueryParserBase;
import org.apache.lucene.queryparser.flexible.core.nodes.PathQueryNode.QueryText;
import org.apache.lucene.search.FuzzyQuery;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.PhraseQuery;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.TotalHitCountCollector;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.RAMDirectory;

public class PhaseQueryTest {
	private Directory dir;
	private IndexSearcher searcher;
	DirectoryReader reader;
	protected void setUp() throws IOException {
		dir = new RAMDirectory();
		IndexWriter writer = new IndexWriter(dir, new IndexWriterConfig(new StandardAnalyzer()));
		Document doc = new Document();
		doc.add(new Field("contents", "the quick brown "
				+ "fox jumped over the lazzy dog", TextField.TYPE_STORED));
		writer.addDocument(doc);
		writer.close();
		reader = DirectoryReader.open(dir);
		searcher = new IndexSearcher(reader);
	}
	static String index_path = "/Users/apple/prg/picfea/index";
	protected void build() throws IOException {
		if (new File(index_path).listFiles().length > 2) {
			return ;
		}
		dir = FSDirectory.open(Paths.get(index_path));
		IndexWriter writer = new IndexWriter(dir, new IndexWriterConfig(new StandardAnalyzer()));
		File file = new File(index_path.substring(0,index_path.lastIndexOf('/')));
		for (File f: file.listFiles()) {
			if (f.isFile() && f.canRead()) {
				Document doc = new Document();
				FileInputStream fileInputStream = new FileInputStream(f);
				InputStreamReader reader = new InputStreamReader(fileInputStream);
				BufferedReader bReader = new BufferedReader(reader);
				StringBuilder sb = new StringBuilder();
				String line;
				while ((line = bReader.readLine()) != null) {
					sb.append(line);
				}
				doc.add(new Field("filename", f.getName(), TextField.TYPE_STORED));
				doc.add(new Field("contents", sb.toString(), TextField.TYPE_STORED));
				
				writer.addDocument(doc);
			}
		}
		writer.close();
	}
	public static void main(String[] args) throws Exception {
		PhaseQueryTest p = new PhaseQueryTest();
		p.build();
		p.testSearch();
	}
	
	private void testSearch() throws IOException, ParseException {
		if (dir == null) 
			dir = FSDirectory.open(Paths.get(index_path));
		searcher = new IndexSearcher(DirectoryReader.open(dir));
		QueryParser queryParser = new QueryParser("contents", new StandardAnalyzer());
		String queryText = "Nbge ff fc bd de beb Ncmp hi ff gc bd Nbcb fc bd de hb ec hc hc hc gc ke Ndrf fe ff ff gc hc hc hd fh fe fe ff ga uv ff ff ff ff gf fe ff ff Nfad ";
		Query q = queryParser.parse(queryText);
		TopDocs match = searcher.search(q, 10);
		System.out.println(match.scoreDocs.length);
		for (ScoreDoc tp: match.scoreDocs) {
			System.out.println(searcher.doc(tp.doc).get("filename"));
			System.out.println(tp);
		}
	}

	private boolean matched() throws IOException {
		QueryParser queryParser = new QueryParser("contents", new StandardAnalyzer());
		FuzzyQuery fuzzyQuery = new FuzzyQuery(new Term("contents", "quickop"), 2);
		TopDocs matchedDocs = searcher.search(fuzzyQuery, 10);
//		PhraseQuery phraseQuery = queryParser.createMinShouldMatchQuery("contents", queryText, fraction)；
		System.out.println(matchedDocs.totalHits);
		return  matchedDocs.totalHits > 0;
	}
	
	private void minShouldMatch() throws IOException {
		QueryParser queryParser = new QueryParser("contents", new StandardAnalyzer());
		Query q = queryParser.createMinShouldMatchQuery("consents", " quick brown "
				+ "fox jumped over the lazzy dog", (float) 1);
		TopDocs mDocs = searcher.search(q, 10);
		System.out.println(mDocs.totalHits);
	}
	
	public void tearDown() throws IOException {
		reader.close();
		dir.close();
	}
}

