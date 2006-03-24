package utilities;

import simulation.*;

public class MapPoint {
	public int x;
	public int y;

	public MapPoint() {
		x = -1;
		y = -1;
	}
	
	public MapPoint(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public MapPoint(MapPoint p) {
		this.x = p.x;
		this.y = p.y;
	}
	
	public void travel(int direction) {
		x += getXIncrement(direction);
		y += getYIncrement(direction);
	}
	
	public static int getXIncrement(int direction) {
		switch (direction) {
		case WorldEntity.kWestInt:
			return -1;
		case WorldEntity.kEastInt:
			return 1;
		default:
			break;
		}		
		return 0;
	}
	
	public static int getYIncrement(int direction) {
		switch (direction) {
		case WorldEntity.kNorthInt:
			return -1;
		case WorldEntity.kSouthInt:
			return 1;
		default:
			break;
		}		
		return 0;
	}
}
