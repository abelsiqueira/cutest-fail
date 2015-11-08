CFLAGS = -ggdb -Wno-unused
FC = gfortran
SIF = ELFUN EXTER GROUP RANGE
LIB = -L$(CUTEST)/objects/pc64.lnx.gfo/double -lcutest -lgfortran -lgfortranbegin -lpthread -lm -ldl

all: sifdecode exec

sifdecode:
	@$(SIFDECODE)/bin/sifdecoder $(MASTSIF)/HS4.SIF
	mv OUTSDIF.d OUTSDIFHS4.d
	@gfortran -fPIC -c $(addsuffix .f,$(SIF))
	@gfortran -shared -o libHS4.so $(addsuffix .o,$(SIF))
	@$(SIFDECODE)/bin/sifdecoder $(MASTSIF)/HS32.SIF
	mv OUTSDIF.d OUTSDIFHS32.d
	@gfortran -fPIC -c $(addsuffix .f,$(SIF))
	@gfortran -shared -o libHS32.so $(addsuffix .o,$(SIF))
	rm -f $(addsuffix .f,$(SIF)) $(addsuffix .o,$(SIF)) AUTOMAT.d

exec: main.o
	gcc $^ -o main $(LIB) $(CFLAGS) -Wl,--no-as-needed
	LD_LIBRARY_PATH=. ./main

exechs4: main.o libHS4.so
	gcc $^ -o main $(LIB) $(CFLAGS) -Wl,--no-as-needed
	@echo "Running"
	LD_LIBRARY_PATH=. ./main

exechs32: main.o libHS32.so
	gcc $^ -o main $(LIB) $(CFLAGS) -Wl,--no-as-needed
	@echo "Running"
	LD_LIBRARY_PATH=. ./main

main.o: main.c
	gcc -c -Wall -Wextra -I$(CUTEST)/include main.c -o main.o $(CFLAGS)

clean:
	rm -f *.o $(addsuffix *.f,$(SIF)) *.SIF OUTSDIF*.d

purge: clean
	rm -f main *.so
