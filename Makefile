export CC = g++
INCPATH = -I$(CURDIR)/include 
LIBS = -lmgf -ldl -lmonitoring -lsocket
#-lpthread 
#-lmysqlcppconn 
export DEFINES = -DCOLORS 
export FLAGS = -o3 -std=c++0x $(INCPATH) $(LIBS) $(DEFINES)
export TOP = $(CURDIR)
export OBJ_DIR = $(TOP)/obj
export SRC = $(wildcard *.c*)
export OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

export LIB=libharpe-algo
export SHARED=$(LIB).so
export STATIC=$(LIB).a

SUBDIRS = src obj

export EXEC = Harpe-algo


CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)


static : src
	$(MAKE) static -C obj

shared :
	$(MAKE) -C src FLAGS="$(FLAGS) -fPIC"
	$(MAKE) shared -C obj


install :
	cp -f $(STATIC) /usr/local/lib/$(STATIC)
	cp -f $(SHARED) /usr/local/lib/$(SHARED)
	cp -rf include/harpe-algo /usr/local/include/harpe-algo

uninstall:
	rm -f /usr/local/lib/$(STATIC)
	rm -f /usr/local/lib/$(SHARED)
	rm -rf /usr/local/include/harpe-algo

exe: $(OBJ) subdirs

doc : doc/html 

doc/html :
	cd doc && doxygen

subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) -C $@


.cpp.o:
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"

.c.o :
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"


clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f $(EXEC)

