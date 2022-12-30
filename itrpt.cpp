#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>



int main() {
  std::atomic<bool> interrupted;
  int x;
  int i = 0;
  char tt[] = {"|","\","-","/"};

  do {
    interrupted.store(false);

    // create a new thread that does stuff in the background
    std::thread th([&]() {
        while(!interrupted) {
	  
          // do stuff. Just as an example:
	  if(i==x){
	  x=0;
	  std::cout << i <<"--" << std::endl;
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }}
      });

    std::cin >> x;

    // when input is complete, interrupt thread and wait for it to finish
    interrupted.store(true);
    th.join();

    // apply x, then loop to make new thread to do stuff
    i = x;
  } while(x != -1); // or some other exit condition
}
