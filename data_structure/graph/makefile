PROGRAMS = AirlineGraph \
	AirlineDFS \
    AirlineBFS \
	dijkstra

all: $(PROGRAMS)

AirlineBFS: AirlineBFS.o
	g++ -o AirlineBFS AirlineBFS.o 

AirlineDFS: AirlineDFS.o
	g++ -o AirlineDFS AirlineDFS.o 

AirlineGraph: AirlineGraph.o
	g++ -o AirlineGraph AirlineGraph.o 

AirlineBFS.o: AirlineBFS.cpp graphtypes.h
	g++ -c AirlineBFS.cpp

AirlineDFS.o: AirlineDFS.cpp graphtypes.h
	g++ -c AirlineDFS.cpp

AirlineGraph.o: AirlineGraph.cpp graphtypes.h
	g++ -c AirlineGraph.cpp

	
dijkstra: dijkstra.o
	g++ -o dijkstra dijkstra.o

dijkstra.o: dijkstra.cpp graphtypes.h
	g++ -c dijkstra.cpp


clean: 
	rm -f *.o *.a $(PROGRAMS)
