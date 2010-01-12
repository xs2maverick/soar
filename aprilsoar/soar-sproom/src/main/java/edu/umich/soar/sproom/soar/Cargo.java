package edu.umich.soar.sproom.soar;

import edu.umich.soar.sproom.command.VirtualObject;

public class Cargo {
	
	private VirtualObject carriedObject;

	public boolean isCarrying() {
		return carriedObject != null;
	}

	public void setCarriedObject(VirtualObject object) {
		carriedObject = object;
	}
	
	public VirtualObject getCarriedObject() {
		return carriedObject;
	}

}
