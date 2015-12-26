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
	String libpath = "."+ File.separator +"picLib" + File.separator + "index";
	Directory directory;
	public FeaLib() throws IOException {
		directory = FSDirectory.open(Paths.get(libpath));
		IndexWriter indexWriter = new IndexWriter(directory, new IndexWriterConfig(new StandardAnalyzer()));
		indexWriter.close();
	}
	
	public FeaLib(String libpath) throws IOException {
		this.libpath = libpath;
		directory = FSDirectory.open(Paths.get(libpath));
		IndexWriter indexWriter = new IndexWriter(directory, new IndexWriterConfig(new StandardAnalyzer()));
		indexWriter.close();
	}
	
	public void addFeature(PicFeature picFeature) throws IOException, ParseException {
		if (picFeature.getID() == null) {
			System.out.println("The ID is fault");
			return;
		}
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		IndexReader reader = searcher.getIndexReader();
		for (int i = 0; i < reader.maxDoc(); i++) {
			if (picFeature.getID().equals(reader.document(i).get("ID"))) {
				System.out.println("The file already in the lib");
				reader.close();
				return;
			}
		}
		reader.close();
		IndexWriter writer = new IndexWriter(directory, new IndexWriterConfig(new StandardAnalyzer()));
		writer.addDocument(picFeature.getDocument());
		writer.close();
		System.out.println(picFeature.getFileName() + " has been input into the lib");
	}
	
	public void removeFeature(PicFeature picFeature) {
		throw new NotImplementedException();
	}
	
	public SearchRes search(PicFeature pf) throws IOException, ParseException {
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		QueryParser queryParser = new QueryParser(PicFeature.sentencecode, new StandardAnalyzer());
		Query query = queryParser.parse(pf.getSentenceCode());
		IndexReader iReader = searcher.getIndexReader();
		
		TopDocs tDocs = searcher.search(query, iReader.numDocs()/10 > 0 ? iReader.numDocs()/10 : 10);
		double similarity = 0.0;
		PicFeature picFeature = null;
		System.out.println("tDocs.totalHits:" + tDocs.totalHits);
		for (int i = 0; i < tDocs.totalHits; i++) {
			PicFeature feature = new PicFeature(iReader.document(tDocs.scoreDocs[i].doc));
			double sim = feature.computeDistance(pf, 0.8);
			if (sim > similarity) {
				similarity = sim;
				picFeature = feature;
			}
		}
		if (picFeature == null) {
			return new SearchRes(null, Double.MAX_VALUE);
		}
		return new SearchRes(picFeature.getID(), 1.0/similarity);
	}

	public void list() throws IOException {
		IndexSearcher searcher = new IndexSearcher(DirectoryReader.open(directory));
		IndexReader iReader = searcher.getIndexReader();
		System.out.println("there are " + iReader.numDocs() + " documents in this lib");
	}
}