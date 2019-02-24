javac:
	javac -encoding UTF-8 org/libpointing/*.java

javah:
	javac -h native org/libpointing/DisplayDevice.java
	javac -h native org/libpointing/PointingDevice.java
	javac -h native org/libpointing/PointingDeviceManager.java
	javac -h native org/libpointing/DisplayDeviceManager.java
	javac -h native org/libpointing/TransferFunction.java
	javac -h native org/libpointing/TimeStamp.java

clean:
	rm org/libpointing/*.class
	rm native/org*.h
	cd pro/ && make clean

distclean: clean
	rm -rf build/
	rm libpointing.jar

jar:
	jar cvf libpointing.jar org/libpointing/*.class org/libpointing/event/*.class

java: javac javah

nat:
	cd pro/ && qmake && make

all: javac javah jar nat
