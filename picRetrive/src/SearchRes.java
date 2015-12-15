
public class SearchRes {
	private byte[] id;
	private float distance;
	
	public SearchRes(byte[] id, float distance) {
		this.id = id;
		this.distance = distance;
	}
	
	public byte[] getID() {
		return id;
	}
	
	public float getDistance() {
		return distance;
	}
}
