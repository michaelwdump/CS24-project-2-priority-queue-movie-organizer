CPP = g++ -std=c++11
CPPFLAGS = -Wno-unitialized

#runmovies doesn't include movies.o since it's just a pre-compiled header file, not an object file
runMovies: main.o
	$(CPP) $(CPPFLAGS) $^ -o $@

main.o: main.cpp 
	$(CPP) $(CPPFLAGS) -c $^

movies.o: movies.h movies.cpp
	$(CPP) $(CPPFLAGS) -c $^

#movies.o: movies.h
#	$(CPP) $(CPPFLAGS) -c $^

clean:
	rm *.o
