export CC = g++
INCPATH = -I$(CURDIR)/include 
LIBS = -lmgf -ldl
#-lpthread 
#-lmysqlcppconn 
export DEFINES = -DCOLORS 
export FLAGS = -g -std=c++0x $(INCPATH) $(LIBS) $(DEFINES)
export TOP = $(CURDIR)
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

SUBDIRS = src obj

export EXEC = Harpe-algo


CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)

all: $(OBJ) subdirs

subdirs: $(SUBDIRS)

doc : doc/html 

doc/html :
	cd doc && doxygen

calc_score: calc_score_func

calc_score_func:
	$(MAKE) shared -C calc_score

     
$(SUBDIRS):
	$(MAKE) -C $@


.cpp.o:
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"

.c.o :
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ "$(OBJ_DIR)"


clean: $(CLEANDIRS) calc_score

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f *.so
	@rm -f $(EXEC)

