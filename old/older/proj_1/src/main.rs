use p2p_app::*;

fn main () {
    let args = do_args();
    println!( "config file: {}", &(args.config_file) );

    let conf_file = get_config();
}
