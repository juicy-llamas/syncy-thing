use std::env;

pub struct Arguments {
	pub config_file: String
}

#[allow(unused_variables)]
pub fn do_args () -> Arguments {
	let args: Vec<String> = env::args().collect();
	let mut i = 1;
	// Specifies default args
	let mut ret = Arguments{
		config_file: String::from( "./app_config.conf" )
	};

	while i < args.len() {
		match args[ i ].as_str() {
			"-c" => {
				i += 1;
				ret.config_file = args[ i ].clone();
			}
			default => { panic!( "invalid arguments" ); }
		}
		i += 1;
	}

	ret
}

pub fn get_config ( args: Arguments ) -> Result<Config, &str> {
	let conf_file = match OpenOptions::new().read( true ).open( args.config_file ) {
		Ok( fp ) => fp,
		Err( aAaA ) => return gen_config()	// generates the config file and returns default args
	};
	let conf_reader = BufReader::new( conf_file ).bytes();

	let mut ret = Config{}

}

fn absorb_addr ( it:  ) -> ( String, Vec<( String, isize )> ) {
	let hname = absorb_string( it );
	if it.next() == '{' {
		let files: Vec<files> =
		while absorb_file( it )
		( hname, files )
	} else {
		panic!( "config syntax error: no bracket" );
	}
}

fn absorb_file ( it ) -> Result<( {
	if it.next() == '\'' {
		let fname = absorb_string( it );
		if it.next() == '\'' && it.next() == ':' {
			let fperms = absorb_int( it );
			if it.next() != ',' {
				return ( fname, fperms )
			} else {
				panic!( "config syntax error: no comma" );
			}
		} else {
			panic!( "config syntax error: no colon/string not terminated" ).
		}
	} else {
		return None
	}
}

fn gen_config () -> Result<Config, &str> {
	let mut conf_file = match OpenOptions::new().create( true ).write( true ).open( args.config_file ) {
		Ok( fp ) => fp,
		Err( aAaA ) => Err( aAaA )
	};
	static default_config = Config {

	};
}

pub struct SharedFile {
	pub name: String,
	pub perms: i32,
	// fp?
}

pub struct Hosts {
	pub name: String,
	pub files: Vec<SharedFile>
	// extra stuff? like methods, callbacks, etc
}

pub struct Config {
	pub hosts: Vec<Hosts>
	// anything else?
}
