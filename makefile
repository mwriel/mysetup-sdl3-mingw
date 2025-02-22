# Variables
CXX = g++ 									# Compilador
CXXVER = -std=c++20                               
CXXFLAGS = -I src/include     				# Flags para el compilador (versión y encabezados)
LDFLAGS = -L src/lib                  	   	# Flags para el enlazador (directorios de librerías)
LIBS = -lmingw32 -lSDL3                	  	# Librerías necesarias

LIB_HEADERS = $(wildcard src/include/*.h)
LIB_FILES = $(wildcard src/lib/*.a src/lib/*.so SDL3.dll)

# Archivos fuente
SOURCES = m.c myFirstSDLApp.c
# Archivos objeto (se generan a partir de las fuentes)
OBJECTS = $(SOURCES:.c=.o)

# Ejecutables
EXECS = claro.exe myFirstSDLApp.exe primitivos.exe

# Regla por defecto
default: 
	para crear propiamente la practica hay que ejecutar make graficas.exe
	aparte para modificar el algoritmo se necesita modificar el archivo drawit.c
	si decea añadir otra funcion en graficas.exe se recomienda añadirla a drawit.h
	definirla en drawit.c y ejecutar el mismo comando

# Crear el ejecutable 'new.exe'
#claro.exe: claro.o $(LIB_HEADERS) $(LIB_FILES)
#	$(CXX) claro.o -o claro.exe $(LDFLAGS) $(LIBS)
	
#claro.exe: claro.o $(LIB_HEADERS) $(LIB_FILES)
#	$(CXX) claro.o -o claro.exe $(LDFLAGS) $(LIBS)

# Crear el ejecutable 'myFirstSDLApp.exe'
#myFirstSDLApp.exe: myFirstSDLApp.o $(LIB_HEADERS) $(LIB_FILES)
#	$(CXX) myFirstSDLApp.o -o myFirstSDLApp.exe $(LDFLAGS) $(LIBS)

# Regla genérica para generar archivos .o
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.out.exe: %.o
	
	$(CXX) $< -o $@ $(LDFLAGS) $(LIBS)

# Limpiar archivos generados
clean:
	rm -f *.o $(EXECS)
graficasdeprecated.exe: graficas.o drawit.o

	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)
say: graficas.o  drawit.o
	echo $^

graficas.%.exe: graficas.o %.o

	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)
