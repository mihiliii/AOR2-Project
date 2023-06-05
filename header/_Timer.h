#pragma once
#include <string.h>
#include <chrono>


#define StartTimer(name)	{ const char* timername = #name;\
				auto start = std::chrono::steady_clock::now();

#define EndTimer  auto end =  std::chrono::steady_clock::now();	\
					if(strlen(timername)>0)			\
						std::cout<<""<<timername<<": \t"<< chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns" <<std::endl; \
					else \
						std::cout<<"Time of execution: %f\n" << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << " ns" <<std::endl; \
						} \


