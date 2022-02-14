CXX = g++
CXXFLAGS = -pedantic --std=c++11 -Wall -Werror -Wextra -g
TARGETS = client server proxy
CLIENT_OBJS = client.o
SERVER_OBJS = server.o
PROXY_OBJS = proxy.o httpParser.o

all: $(TARGETS)
client: $(CLIENT_OBJS) cliet-test.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(CLIENT_OBJS) cliet-test.cpp
server: $(SERVER_OBJS) server-test.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(SERVER_OBJS) server-test.cpp
proxy: $(PROXY_OBJS) 
	$(CXX) -o $@ $(CXXFLAGS) $(PROXY_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS) -lpthread

httpParser: $(HTTP_OBJS) httpParser.cpp httpParser.hpp

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ *.o
