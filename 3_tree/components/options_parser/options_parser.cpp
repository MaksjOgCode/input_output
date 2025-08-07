#include "options_parser.hpp"
#include <boost/program_options.hpp>


namespace po = boost::program_options;



 std::vector <std::string> expand_grouped_flags ( int argc, char* argv[] );



options parse ( int argc, char* argv[] ) {
    auto argv2 = expand_grouped_flags(argc, argv);

    options opts;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h",    "show help")
        ("permissions,p", po::bool_switch( &opts.show_permissions), "show file permissions" )
        ("sizes,s",       po::bool_switch( &opts.show_sizes), "show file sizes"             )
        ("include,P", po::value <std::vector <std::string> > ( &opts.include_patterns )->multitoken(), "include patterns")
        ("exclude,I", po::value <std::vector <std::string> > ( &opts.exclude_patterns )->multitoken(), "exclude patterns")
        ("path",      po::value <std::string> ( &opts.root_path )->default_value("."), "root directory");

    po::positional_options_description pos;
    pos.add("path", 1);

    po::variables_map vm;
    std::vector <std::string> args;
    for (size_t i = 1; i < argv2.size(); ++i) { args.push_back(argv2[i]); }
    po::store ( po::command_line_parser(args).options(desc).positional(pos).run(), vm );

    if ( vm.count( "help" ) ) {
        std::cout << desc << "\n";
        std::exit(0);
    }
    po::notify(vm);

    return opts;
}

 std::vector <std::string> expand_grouped_flags ( int argc, char* argv[] ) {
    std::vector<std::string> out;
    /* argv[0] — NAME PROGRAM: */
    out.emplace_back(argv[0]);

    for ( int i = 1; i < argc; ++i ) {
        std::string a = argv[i];
        if ( a.size() > 2 && a[0] == '-' && a[1] != '-' && a.find('=') == std::string::npos ) {
            bool can_expand = true;
            for ( size_t j = 1; j < a.size(); ++j ) {
                char c = a[j];
                if ( c != 'p' && c != 's' ) { can_expand = false; break; }
            }
            if ( can_expand ) {
                for ( size_t j = 1; j < a.size(); ++j ) { out.push_back( std::string("-") + a[j] ); }
                continue;
            }
        }
        out.push_back(a);
    }

    return out;
}