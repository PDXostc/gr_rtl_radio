.PHONY : clean all install

TARGET = libgr_rtl_radio.so
TARGET_RECEIVER = udp_receiver
DESTDIR ?= /usr/local

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
DEV_HDR = ./include/gr_rtl_tuner.h
LDFLAGS = -shared -Wl,-rpath=/usr/lib/x86_64-linux-gnu
CXXFLAGS = -fPIC -Wall -Wextra
DEBUGFLAGS = -g
RELEASEFLAGS = -O3

SOURCES = $(shell echo ./src/*.cpp)
HEADERS = $(shell echo ./include*.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET) $(TARGET_RECEIVER)

install: all
	install -d ${DESTDIR}/lib; \
	install -d ${DESTDIR}/include; \
	install -m 0644 ${TARGET}  ${DESTDIR}/lib; \
	install -m 0644 ${DEV_HDR}  ${DESTDIR}/include;

$(TARGET_RECEIVER): 

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(INC_PATHS) -c $*.cpp -o $*.o

clean:
	-rm -f ${TARGET} ${OBJECTS}
