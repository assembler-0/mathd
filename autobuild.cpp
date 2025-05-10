#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "../include/fmt/core.h"
void gen(){
  char mode;
  printf("Please choose builder: [N]inja or [M]ake: ");
  std::cin >> mode;
  switch(mode){
    case('N'): system("cd build && cmake .. -G Ninja"); break;
    case('M'): system("cd build && cmake .."); break;
    default: printf("Please choose a valid option, stopping.."); break;
  } 
  return;
}
int main(){
  printf("Preparing to clean build/* ");
  system("rm -rf build/*");
  gen();
  printf("Building..");
  system("cd build && make -j8");
  return 0;
}
