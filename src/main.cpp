#include "../include/smctl_calculator.hpp"
#include "../include/CLI11.hpp"
void init_calculator(int argc, char** argv){
  (void)argc;
  (void)argv;
  Calculator scicalc;
  scicalc.run();
  return;
}
void init_calculator_high_precision(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'hpc' (High Precision/Performance Calculator) command is not yet implemented." << std::endl;
  return;
}
void init_cache_cleaner(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'cc' (cache cleaner) command is not yet implemented." << std::endl;
  return;
}
void init_file_marker(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'mark' command is not yet implemented." << std::endl;
  return;
}
void init_auto_update(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'au' command is not yet implemented." << std::endl;
  return;
}
void init_system_autotune(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'autotune' command is not yet implemented." << std::endl;
  return;
}
void init_system_resource_manager(int argc, char** argv){
  (void)argc;
  (void)argv;
  std::cout << "The 'rm' (Resource Manager) command is not yet implemented." << std::endl;
  return;
}
int main(int argc, char **argv){
  CLI::App smctl{"smctl - A Powerful system manager for Arch-based distro"};
  smctl.name("smctl");
  smctl.set_version_flag("-v,--version", "smctl version 2.0 - Remastered", "Display version information");
  CLI::App* calc_subcommand = smctl.add_subcommand("calc", "Perform mathematical and graphing calculations");
  calc_subcommand->fallthrough();
  calc_subcommand->callback([argc, argv](){
      init_calculator(argc, argv);
  });
  CLI::App* hpc_subcommand = smctl.add_subcommand("hpc", "High Precision/Performance Calculator (no graphing)");
  hpc_subcommand->fallthrough();
  hpc_subcommand->callback([argc, argv](){
      init_calculator_high_precision(argc, argv);
  });
  CLI::App* ccleaner_subcommand = smctl.add_subcommand("cc", "Cache and garbage collection for pacman and marked git repos");
  ccleaner_subcommand->fallthrough();
  ccleaner_subcommand->callback([argc, argv]() {
      init_cache_cleaner(argc, argv);      
  });
  try {
    smctl.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return smctl.exit(e);
  }
  if (argc == 1) {
    std::cout << smctl.help() << std::endl;
    return 0;
  }
  return 0;
}
