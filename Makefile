.PHONY : clean

TARGET = libgr_rtl_radio.so
 
LIBS = -lgnuradio-runtime \
       -lgnuradio-osmosdr \
       -lgnuradio-pmt \
       -lgnuradio-filter \
       -lgnuradio-audio \
       -lgnuradio-analog \
       -lgnuradio-blocks \
       -pthread \
       -lboost_system

INC_PATHS = -I./include
LDFLAGS = -shared
CXXFLAGS = -fPIC 
DEBUGFLAGS = -g
RELEASEFLAGS = -O3

SOURCES = $(shell echo ./src/*.cpp)
HEADERS = $(shell echo ./include*.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(INC_PATHS) -c $*.cpp -o $*.o

clean:
	-rm -f ${TARGET} ${OBJECTS}
