
use std::io::SocketAddr;
use std::net::{ IpAddr, Ipv4Addr, Ipv6Addr };
use chrono::{ DateTime, TimeZone };

pub mod util {

	fn pt ( hm: String, m: String ) {
		let stamp: DateTime<Local> = Local::now();
		println!( "[{}] {}: {}", stamp.to_rfc2822(), hm, m );
	}
	fn ept ( hm: String, m: String, e: Error ) {
		let stamp: DateTime<Local> = Local::now();
		println!( "[{}] {}: {}: {}", stamp.to_rfc2822(), hm, m, e );
	}

	#[macro_export]
	macro_rules! warn {
		( m, e ) => ept( "Warning", m, e )
	}

	#[macro_export]
	macro_rules! error {
		( m, e ) => ept( "Error", m, e )
	}

	#[macro_export]
	macro_rules! fatal {
		( m, e ) => {
			ept( "Fatal Error", m, e )
			panic!( "Terminated for reasons above." );
		}
	}

	#[macro_export]
	macro_rules! log {
		( m ) => if verbose {
			pt( "Log", m )
		}
	}

	#[macro_export]
	macro_rules! important {
		( m ) => pt( "Important", m )
	}

	#[macro_export]
	macro_rules! ip_ad {
		( $a:u8, $b:u8, $c:u8, $d:u8, $p:u16 ) => {
			SocketAddr::new( IpAddr::V4( Ipv4Addr::new( a, b, c, d ) ), p )
		},
		( $st:str ) => {
			let res = str.parse::<SocketAddr>();
			match res {
				Ok( a: SocketAddr ) => a,
				// perhaps change in the future
				Err( e ) => panic!( "Error parsing IP: {}", e )
			}
		},
		( $a:u16, $b:u16, $c:u16, $d:u16, $e:u16, $f:u16, $g:u16, $h:u16, $p:u16 ) => {
			SocketAddr::new( IpAddr::V6( Ipv6Addr::new( a, b, c, d, e, f, g, h ) ), p )
		},
		default => panic!( "You used the macro incorrectly! Fuck you!" );
	}
}