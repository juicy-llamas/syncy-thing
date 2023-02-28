
mod network;
use crate::network::{ start_server, start_client };

fn main() {
    if let Err( e ) = start_server() {
        eprintln!( "Server didn't start because of {}", e );
        match start_client() {
            Ok( hand ) => { hand.join(); println!( "All done" ) },
            Err( e ) => eprintln!( "Client didn't start because of {}", e )
        }
    }
}
