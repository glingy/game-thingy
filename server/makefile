EXAMPLE_FILES := HelloWorld ServerName EchoServer BroadcastingEchoServer UpgradeSync UpgradeAsync
THREADED_EXAMPLE_FILES := HelloWorldThreaded EchoServerThreaded
override CXXFLAGS += -lpthread -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++20 -Ilib/uWebSockets/uSockets/src -Ilib/uWebSockets/src
override LDFLAGS += lib/uWebSockets/uSockets/uSockets.a -lz -lmysqlclient -lstdc++
CXX := g++-10
CC := gcc-10

DESTDIR ?=

# WITH_PROXY enables PROXY Protocol v2 support
ifeq ($(WITH_PROXY),1)
	override CXXFLAGS += -DUWS_WITH_PROXY
endif

# WITH_OPENSSL=1 enables OpenSSL 1.1+ support
ifeq ($(WITH_OPENSSL),1)
	# With problems on macOS, make sure to pass needed LDFLAGS required to find these
	override LDFLAGS += -lssl -lcrypto
else
	# WITH_WOLFSSL=1 enables WolfSSL 4.2.0 support (mutually exclusive with OpenSSL)
	ifeq ($(WITH_WOLFSSL),1)
		override LDFLAGS += -L/usr/local/lib -lwolfssl
	endif
endif

# WITH_LIBUV=1 builds with libuv as event-loop
ifeq ($(WITH_LIBUV),1)
	override LDFLAGS += -luv
endif

# WITH_ASAN builds with sanitizers
ifeq ($(WITH_ASAN),1)
	override CXXFLAGS += -fsanitize=address -g
	override LDFLAGS += -lasan
endif


SRCS := $(wildcard src/*.cpp)
FILES := $(patsubst %.cpp,%,$(SRCS))
OBJS := $(patsubst src/%.cpp,build/%.o,$(SRCS))

.PHONY: server

server: _build $(OBJS)
	@if [ ! -f lib/uWebSockets/uSockets/uSockets.a ]; then $(MAKE) -s -C lib/uWebSockets/uSockets; fi
	@$(CXX) -flto $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o build/server
	@echo "Success"

build/%.o: src/%.cpp
	@$(CXX) -flto $(CXXFLAGS) $< -c -o $@

_build:
	@mkdir -p build

prod:
	@$(MAKE) -s -C lib/uWebSockets/uSockets > /dev/null
	@mkdir -p build
	@for FILE in $(patsubst src/%,%,$(FILES)); do $(CXX) -flto -O3 $(CXXFLAGS) src/$$FILE.cpp -c -o build/$$FILE.o ;done
	@$(CXX) -flto -O3 $(CXXFLAGS) $(patsubst src/%,build/%.o,$(FILES)) $(LDFLAGS) -o build/server;

clean:
	rm -rf build