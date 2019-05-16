.PHONY : clean all install

TARGET = libgr_rtl_radio.so
DESTDIR ?= /usr/local

LIBSLIST = -lgnuradio-runtime \
       -lgnuradio-osmosdr \
       -lgnuradio-pmt \
       -lgnuradio-filter \
       -lgnuradio-audio \
       -lgnuradio-analog \
       -lgnuradio-blocks \
       -lgnuradio-digital \
       -lgnuradio-rds \
       -pthread \
       -lboost_system \
       $(LIBS)

INC_PATHS = -I./include -I/usr/local/include/rds/gnuradio
DEV_HDR = ./include/gr_rtl_tuner.h
LDFLAGSLIST = -shared -Wl,-rpath=/usr/lib/x86_64-linux-gnu $(LDFLAGS)
CXXFLAGSLIST = -fPIC -Wall -Wextra -std=c++11 $(CXXFLAGS) $(CPPFLAGS)
DEBUGFLAGS = -g
RELEASEFLAGS = -O3

export CXXFLAGSLIST
export LIBSLIST
export LDFLAGSLIST

SOURCES = $(shell echo ./src/*.cpp)
HEADERS = $(shell echo ./include*.h)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

install: all
	install -d ${DESTDIR}/lib; \
	install -d ${DESTDIR}/include; \
	install -m 0644 ${TARGET}  ${DESTDIR}/lib; \
	install -m 0644 ${DEV_HDR}  ${DESTDIR}/include;

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGSLIST) $(LDFLAGSLIST) -o $@ $^ $(LIBSLIST)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGSLIST) $(LDFLAGSLIST) $(RELEASEFLAGS) $(INC_PATHS) -c $*.cpp -o $*.o

clean:
	-rm -f ${TARGET} ${OBJECTS}
