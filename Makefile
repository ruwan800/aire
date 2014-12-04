CC=g++
C_INCLUDES=`pkg-config --cflags opencv`
CFLAGS=$(C_INCLUDES) -g
LIBS=`pkg-config --libs opencv`

CLAM_CFLAGS=`pkg-config clam_core clam_processing clam_audioio --cflags` -g
CFLAG_VALS=	-DCLAM_FLOAT -DUSE_XERCES=1 -DCLAM_USE_XML -DUSE_LADSPA=1 -DUSE_SNDFILE=1\
			-DUSE_OGGVORBIS=1 -DWITH_VORBIS=1 -DUSE_MAD=1 -DWITH_MAD=1\
 			-DUSE_ID3=1 -DUSE_ALSA=1 -DUSE_JACK=1 -DUSE_PORTAUDIO=1 -DUSE_FFTW3=1

CLAM_LIBS=`pkg-config clam_core clam_processing clam_audioio --libs`

LIBDIR=/usr/lib

BUILD_DIR = build

all: directories libaire.so

directories: ${BUILD_DIR}

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}
	
video.o: directories
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/video.o src/video.cpp

motion.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/motion.o src/motion.cpp

cammotion.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/cammotion.o src/camera_motion.cpp

edge.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/edge.o src/edge.cpp
	
log.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/log.o src/log.cpp
	
io.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/io.o src/io.cpp
	
premining.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/premining.o src/premining.cpp
	
visualize.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/visualize.o src/visualize.cpp
	
objects.o: $(SOURCES)
	g++ -Wall $(CFLAGS) -c -o ${BUILD_DIR}/objects.o src/objects.cpp
	
sound.o: $(SOURCES)
	g++ -Wall $(CLAM_CFLAGS) -c -o ${BUILD_DIR}/sound.o src/sound.cpp

libaire.so: video.o motion.o cammotion.o edge.o log.o io.o premining.o visualize.o objects.o sound.o
	g++ -Wall -shared \
	${BUILD_DIR}/cammotion.o \
	${BUILD_DIR}/motion.o \
	${BUILD_DIR}/video.o \
	${BUILD_DIR}/edge.o \
	${BUILD_DIR}/log.o \
	${BUILD_DIR}/io.o \
	${BUILD_DIR}/premining.o \
	${BUILD_DIR}/visualize.o \
	${BUILD_DIR}/objects.o \
	${BUILD_DIR}/sound.o \
	-o ${BUILD_DIR}/libaire.so $(LIBS) $(CLAM_LIBS)


install:
	rm -f /usr/lib/libaire.so
	rm -rf /usr/include/aire
	rm -f /usr/lib/pkgconfig/aire.pc
	cp ${BUILD_DIR}/*.so /usr/lib
	mkdir /usr/include/aire
	cp src/*.h  /usr/include/aire
	cp aire.pc  /usr/lib/pkgconfig

uninstall:
	rm -f ${BUILD_DIR}/*.o ${BUILD_DIR}/*.so
	rm -rf ${BUILD_DIR}
	rm -f /usr/lib/libaire.so
	rm -rf /usr/include/aire
	rm -f /usr/lib/pkgconfig/aire.pc

clean:
	rm -f ${BUILD_DIR}/*.o ${BUILD_DIR}/*.so
