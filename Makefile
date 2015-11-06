FC = gfortran
SIF = ELFUN EXTER GROUP RANGE
LIB = -lcutest -lgfortran -lgfortranbegin -lpthread -lm

all: sifdecode exechs4 exechs32

sifdecode:
	@$(SIFDECODE)/bin/sifdecoder $(MASTSIF)/HS4.SIF
	mv OUTSDIF.d OUTSDIFHS4.d
	for f in $(SIF); do mv $$f.f $${f}HS4.f; done
	@gfortran -c $(addsuffix HS4.f,$(SIF))
	@$(SIFDECODE)/bin/sifdecoder $(MASTSIF)/HS32.SIF
	mv OUTSDIF.d OUTSDIFHS32.d
	for f in $(SIF); do mv $$f.f $${f}HS32.f; done
	@gfortran -c $(addsuffix HS32.f,$(SIF))

exechs4: main.o $(addsuffix HS4.o,$(SIF))
	@gcc $^ -o main $(LIB)
	@echo "Running"
	./main

exechs32: main.o $(addsuffix HS32.o,$(SIF))
	@gcc $^ -o main $(LIB)
	@echo "Running"
	./main

main.o: main.c
	ls *.o
	gcc -c -Wall -Wextra -I$(CUTEST)/include main.c -o main.o

clean:
	rm -f *.o $(addsuffix *.f,$(SIF)) *.SIF OUTSDIF*.d

purge: clean
	rm -f main
