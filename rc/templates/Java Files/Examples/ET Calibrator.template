import cbccore.sensors.analog.Analog;
import cbccore.sensors.buttons.BlackButton;

class Main {
	public static void main(String[] args) {
		double[] distances = {10., 20., 30., 40., 50., 60., 70., 80., 90., 100.};
		double[] analogvals = new double[10];
		
		System.out.println("using analog port 0");
		Analog distSensor = new Analog(0);
		distSensor.setFloating(true);
		
		BlackButton button = new BlackButton();
		
		for(int i = 0; i < distances.length; ++i) {
			System.out.println("move block "+distances[i]+"cm away");
			System.out.println("then press black button");
			
			//really dirty code, do not follow my example
			while(!button.getValue()) { try { Thread.sleep(50); } catch(Exception e) {} }
			while(button.getValue()) { try { Thread.sleep(50); } catch(Exception e) {} }
			
			int sum = 0;
			
			for(int w = 0; w < 50; ++w) {
				sum += distSensor.getValueHigh();
				try { Thread.sleep(10); } catch(Exception e) {}
			}
			analogvals[i] = sum/50.;
		}
		
		double bestDeviation = Double.POSITIVE_INFINITY;
		double bestPow = -1.;
		double bestK = -1.;
		double bestB = -1.;
		
		for(double i = 0.01; i < 3; i += 0.01) {
			double k = 0.;
			for(int w = 0; w < analogvals.length; ++w) {
				k += distances[w]*Math.pow(analogvals[w], i);
			}
			k /= (double)analogvals.length;
			
			double b = 0.;
			for(int w = 0; w < analogvals.length; ++w) {
				b += distances[w]-k/Math.pow(analogvals[w], i);
			}
			b /= (double)analogvals.length;
			
			double deviation = 0.;
			for(int w = 0; w < analogvals.length; ++w) {
				deviation += Math.abs(distances[w]-k/Math.pow(analogvals[w], i)+b);
			}
			
			if(deviation < bestDeviation) {
				bestDeviation = deviation;
				bestPow = i;
				bestK = k;
				bestB = b;
			}
		}
		
		System.out.println("bestPow: "+ bestPow);
		System.out.println("bestK: "+ bestK);
		System.out.println("bestB: "+ bestB);
		System.out.println("with deviation: "+ bestDeviation/distances.length);
	}
}
