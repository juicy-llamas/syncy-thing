
/**
 * Peer
 * 
 * The base class for all peer connections
 */

use crate::util;
use std::io::IpAddr;
use crate::group::{ Groups, GroupPermissions };

pub struct Peer {
	address: IpAddr,
	hostname: String,
	// groups: Vec<*Groups>,
	policies: Vec<GroupPermissions>,
	// other relevant peer information such as group, file perms, hostname, a crypto info struct, etc

}

impl Peer {
	pub fn new ( addr: IpAddr ) -> Self {
		Peer {
			address: addr.clone(),
			hostname: String::new(),
			// groups: Vec::new(),
			policies: Vec::new()
		}
	}
	pub fn new ( hostname: String ) -> Self {
		Peer {
			address: IpAddr::new(), // resolve hostname?
			hostname: hostname.clone(),
			// groups: Vec::new(),
			policies: Vec::new()
		}
	}

    pub fn send_file ( path:)
}