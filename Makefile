CFLAG = -c -std=c++11 -O2 -Wall -Wextra

bin/pass_sheet: obj/main.o obj/classroom.o | bin
	g++ -o $@ $^

obj/main.o: src/main.cpp src/classroom.h | obj
	g++ $(CFLAG) -o $@ $<

obj/classroom.o: src/classroom.cpp src/classroom.h | obj
	g++ $(CFLAG) -o $@ $<

bin:
	mkdir bin
obj:
	mkdir obj

clean:
	-rm obj/*.o
