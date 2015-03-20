HEADERS = Engine.h AttrDescriptor.h Record.h Node.h
CPPS = Engine.cpp main.cpp

main : $(HEADERS) $(CPPS)
	g++ $(CPPS) -o main
