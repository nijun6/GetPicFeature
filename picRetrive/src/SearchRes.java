import com.sun.xml.internal.bind.v2.runtime.RuntimeUtil.ToStringAdapter;

public class SearchRes {
	private String id;
	private double distance;
	
	public SearchRes(String id, Double distance) {
		this.id = id;
		this.distance = distance;
	}
	
	public String getID() {
		return id;
	}
	
	public double getDistance() {
		return distance;
	}
	
	@Override
	public String toString() {
		return "id = " + id + ",\n distance = " + distance;
	}
}
