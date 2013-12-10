BASE=calc_score
STATIC=$(BASE).a
SHARED=$(BASE).so

#.cpp.o:
#	$(CC) $(FLAGS) -o "$@" -c "$<" 

shared: calc_score.cpp
	$(CC) $(DEFINES) -fPIC -o "calc_score.o" -c "$<" 
	gcc -shared -Wl,-soname,$(TOP)/$(SHARED) -o $(TOP)/$(SHARED) "calc_score.o"


clean:
	@rm -f *.o
	@rm -f *.exe
