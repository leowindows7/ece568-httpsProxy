CXX = g++
CXXFLAGS = -pedantic --std=c++11 -g
TARGETS = proxy
CLIENT_OBJS = client.o
SERVER_OBJS = server.o
PROXY_OBJS = proxy.o httpParser.o

all: $(TARGETS)
proxy: $(SERVER_OBJS) $(CLIENT_OBJS) $(PROXY_OBJS) 
	$(CXX) -o $@ $(CXXFLAGS) $(PROXY_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS) -lpthread

httpParser: $(HTTP_OBJS) httpParser.cpp httpParser.hpp

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -lpthread

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ *.o
