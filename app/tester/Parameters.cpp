// Copyright 2012-2013 Jan de Cuveland <cmail@cuveland.de>

#include "Parameters.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

Parameters::Parameters(int argc, char* argv[]) {
  //  unsigned log_level = 2;
  //  unsigned log_syslog = 2;

  po::options_description generic("Generic options");
  auto generic_add = generic.add_options();
  generic_add("help,h", "display this help and exit");
  generic_add("nosyslog", po::value<bool>(&nosyslog)->implicit_value(true),
              "do not use syslog logger sink");
  generic_add("logfile",
              po::value<std::string>(&logfile)->value_name("<filename>"),
              "write log output to file");
  generic_add("monitor,m",
              po::value<std::string>(&monitor_uri)
                  ->value_name("<uri>")
                  ->implicit_value("http://login:8086/"),
              "publish status to InfluxDB");

  /*
           << "  Default for all LogLevels is Info\n"
           << "  Valid LogLevels are: Trace, Debug, Info, Note, Warning, "
           << " Error, Fatal" << endl;
  */

  po::options_description cmdline_options("Allowed options");
  cmdline_options.add(generic);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
  po::notify(vm);

  if (vm.count("help") != 0u) {
    std::cout << "monitoring tester"
              << "\n";
    std::cout << cmdline_options << std::endl;
    exit(EXIT_SUCCESS);
  }
}
