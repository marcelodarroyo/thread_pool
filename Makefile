thread_pool_example: thread_pool_example.cpp thread_pool.cpp thread_pool.h
	c++ -std=c++11 -o thread_pool_example thread_pool_example.cpp thread_pool.cpp

clean:
	rm thread_pool_example
