#Steven Mike 901849187
#Created by Ramzi Bualuan, edited slightly by me
CMP = gcc
MAIN = final
FUNC = gfxF
EXEC = $(MAIN)
FLAGS = -lm -lX11

$(EXEC): $(MAIN).o $(FUNC).o
	$(CMP) $(MAIN).o $(FUNC).o $(FLAGS) -o $(EXEC)

$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CMP) -c $(MAIN).c

$(FUNC).o: $(FUNC).c
	$(CMP) -c $(FUNC).c

clean:
	rm *.o $(EXEC)

