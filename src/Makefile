CXX = g++
CXXFLAGS = -pedantic --std=c++11 -Wall -Werror -Wextra -g
TARGETS = client server
CLIENT_OBJS = client.o
SERVER_OBJS = server.o
all: $(TARGETS)
client: $(CLIENT_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(CLIENT_OBJS)
server: $(SERVER_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(SERVER_OBJS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ *.o
