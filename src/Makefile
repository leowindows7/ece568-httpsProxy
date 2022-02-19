CXX = g++
CXXFLAGS = -pedantic --std=c++11 -g
TARGETS = proxy
CLIENT_OBJS = client.o
SERVER_OBJS = server.o
PROXY_OBJS = proxy.o httpParser.o
CACHE_OBJS = LRUCache.o
CCONTROL_OBJS = CacheController.o LRUCache.o TimeMaker.o

all: $(TARGETS)
proxy: $(SERVER_OBJS) $(CLIENT_OBJS) $(CCONTROL_OBJS) $(PROXY_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(PROXY_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS) $(CCONTROL_OBJS) -lpthread

httpParser: $(HTTP_OBJS) httpParser.cpp httpParser.hpp

LRUCache: $(CACHE_OBJS) LRUCache_test.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(CACHE_OBJS) LRUCache_test.cpp

CacheController: $(CCONTROL_OBJS) CacheController_test.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(CCONTROL_OBJS) CacheController_test.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(TARGETS) *~ *.o
