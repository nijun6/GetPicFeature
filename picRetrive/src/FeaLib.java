import java.io.IOException;
import java.nio.file.Paths;
import java.util.Collection;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

public class FeaLib {
	public static final String libpath= ".\\piclib";
	
	public static void addDoc(Document doc) throws IOException {
		Directory directory = FSDirectory.open(Paths.get(libpath));
		IndexWriter indexWriter = new IndexWriter(directory, new IndexWriterConfig(new StandardAnalyzer()));
		indexWriter.addDocument(doc);
		indexWriter.close();
	}

	public SearchRes search(PicFeature pf) throws IOException, ParseException {
		Directory directory = FSDirectory.open(Paths.get(libpath));
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		QueryParser queryParser = new QueryParser(PicFeature.sentencecode, new StandardAnalyzer());
		Query query = queryParser.parse(pf.getSentenceCode());
		
		IndexReader iReader = searcher.getIndexReader();
		iReader.getDocCount(PicFeature.filename);
		//searcher.doc(docID);
		return null;
	}
	
	
}
