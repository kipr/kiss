import cbccore.movement.*;
import cbccore.sensors.vision.*;

class Main {
	public static void main(String [] args) {
		Camera c = new Camera();
		
		for(int i = 0; i < 4; ++i) {
			BlobList bList = new BlobList(c, i);
			System.out.println("--------------\nI see " + bList.size() + " blobs on channel " + i);
			
			//individual blobs
			for(Blob k : bList) {
				System.out.println("I see a blob with " + k.getConfindence() +
					"% confidence at position (" + k.getX() + ", " + k.getY() + ").\n\n");
			}
		}
	}
}
