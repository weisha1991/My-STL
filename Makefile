
main:main.o Alloc.o
	g++ -o  main main.o Alloc.o -std=c++0x -fpermissive
main.o:main.cpp
	g++ -c  main.cpp hashtable_test.h Hashtable.h -std=c++0x -fpermissive
Alloc.o:Alloc.cpp
	g++ -c Alloc.cpp -std=c++0x -fpermissive
clean:
	-rm main *.o
