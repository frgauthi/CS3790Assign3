cc = g++ -std=c++11
obj = memalloc.o
bin = memalloc

$(bin): $(obj)
	$(cc) $(obj) -o $(bin)

.cpp.o:
	$(cc) -c $<

clean:
	rm *.o $(bin)



