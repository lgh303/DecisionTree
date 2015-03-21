HEADERS = Engine.h AttrDescriptor.h Record.h Node.h
CPPS = Engine.cpp main.cpp

main : $(HEADERS) $(CPPS)
	g++ -g $(CPPS) -o main

clean :
	rm main
