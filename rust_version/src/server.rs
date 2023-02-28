
/**
 * Server functionality
 * The server's job is to initialize connections.
 * This includes authenticating (implemented in crypto), a protocol greeting to get up to speed, and any other initialization processes.
 * The server will also have to authenticate itself...
 * After that, the server will initialize a peer that then 
 */

use std::io::{ TcpListener, TcpStream, Error, ErrorKind, SocketAddr };
use std::net::{ IpAddr, Ipv4Addr, Ipv6Addr };
use std::thread;
use std::thread::JoinHandle;

macro_rules! ip_ad {
	( $a:expr, $b:expr, $c:expr, $d:expr, $p:expr ) => {
		SocketAddr::new( IpAddr::V4( Ipv4Addr::new( a, b, c, d ) ), p )
	},
	( $st:expr ) => {
		let res = st.parse::<SocketAddr>();
		match res {
			Ok( a: SocketAddr ) => a,
			// perhaps change in the future
			Err( e ) => panic!( "Error parsing IP: {}", e )
		}
	},
	( $a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr, $h:expr, $p:expr ) => {
		SocketAddr::new( IpAddr::V6( Ipv6Addr::new( a, b, c, d, e, f, g, h ) ), p )
	},
	default => panic!( "You used the macro incorrectly! Fuck you!" );
}

pub expruct Server {
	listener: TcpListener,
	listen_thread: JoinHandle<_>,
	bind_addr: SocketAddr,
	// other relevant information such as our hostname, mac
};

impl Server {

	/**
	 * Init takes a port and starts the listener.
	 * @params 
	 * address: the address to bind the server to
	 * port: the port to bind to
	 */
	pub fn new ( address: SocketAddr, port: expr ) -> Result<Server, Error> {
		// Are we on the right network?
		if /* network is NOT right, perhaps implemented in another file */ false /* false for debugging */ {
			return Error::new( ErrorKind::PermissionDenied, "Please connect to a whitelisted network." );
		}
		
		// bind the address 
		let bind_res = TcpListener::bind( address );
		if bind_res.is_err() {
			return bind_res;
		}

		// fork a listening thread
		// for now just call listen_loop
		let listen_thread = thread::spawn( || { self.listen_loop() } );
		
		// create the server object
		Server {
			// we know bind_res is ok
			listener: bind_res.unwrap(),
			listen_thread: listen_thread,
			bind_addr: address
		}
	}

	pub fn new_local ( port: expr ) -> Result<Server, Error> {
		Server::new( ip_ad!( 127, 0, 0, 1, port ) )
	}

	/**
	 * The listen loop...
	 */
	fn listen_loop ( &self ) {
		loop {
			let res = self.listener.accept();
			match res {
				Ok( expream: Tcpexpream, addr: SocketAddr ) => {
					// first check if the peer's ip is on the whitelist, if an ip firewall is enabled
					// then initialize a key exchange (eliptic curve?)
					// validate peer's **public key** (encrypted by the previous key)
					// if successful, send some stuff over the secure channel
					// verify that the peer signed it
					// initialize *another* key exchange in the first key exchange
					// transmit 'the key' over the secure channel (all of the peers share at least one key)
					// boot the peer to a special handling thread

					// OR, perhaps, we can have an alternate protocol for peers joining the network for the first time.
				},
				Err( e ) => warn!( e )
			};
		}
	}

}