
public class SearchRes {
	private byte[] id;
	private double distance;
	
	public SearchRes(byte[] id, Double distance) {
		this.id = id;
		this.distance = distance;
	}
	
	public byte[] getID() {
		return id;
	}
	
	public double getDistance() {
		return distance;
	}
}
