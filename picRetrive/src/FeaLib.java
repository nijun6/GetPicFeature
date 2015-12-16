import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;

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
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;

import sun.reflect.generics.reflectiveObjects.NotImplementedException;

public class FeaLib {
	String libpath= "."+ File.separator +"piclib";
	
	public FeaLib() {
	}
	
	public FeaLib(String libpath) {
		this.libpath = libpath;
	}
	
	private void addDoc(Document doc) throws IOException {
		Directory directory = FSDirectory.open(Paths.get(libpath));
		IndexWriter indexWriter = new IndexWriter(directory, new IndexWriterConfig(new StandardAnalyzer()));
		indexWriter.addDocument(doc);
		indexWriter.close();
	}
	
	public void addFeature(PicFeature picFeature) throws IOException {
		addDoc(picFeature.getDocument());
	}
	
	public void removeFeature(PicFeature picFeature) {
		throw new NotImplementedException();
	}
	
	

	public SearchRes search(PicFeature pf) throws IOException, ParseException {
		Directory directory = FSDirectory.open(Paths.get(libpath));
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		QueryParser queryParser = new QueryParser(PicFeature.sentencecode, new StandardAnalyzer());
		Query query = queryParser.parse(pf.getSentenceCode());
		IndexReader iReader = searcher.getIndexReader();
		
		TopDocs tDocs = searcher.search(query, iReader.numDocs()/10);
		double similarity = 0.0;
		PicFeature picFeature = null;
		for (int i = 0; i < tDocs.totalHits; i++) {
			PicFeature feature =new PicFeature(iReader.document(tDocs.scoreDocs[i].doc));
			double sim = feature.computeDistance(pf, 0.8);
			if (sim > similarity) {
				similarity = sim;
				picFeature = feature;
			}
		}
		return new SearchRes(picFeature.getID(), 1.0/similarity);
	}
	
	
}
