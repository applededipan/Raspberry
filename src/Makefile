CC=arm-linux-gnueabihf-gcc
main: main.o wiringSerial.o task1_mavdata.o init.o task2_videodata.o task3_mavpddl.o
	$(CC)  main.o wiringSerial.o task1_mavdata.o init.o task2_videodata.o task3_mavpddl.o -o main

main.o: main.c 
	$(CC) -c main.c

wiringSerial.o: wiringSerial.c wiringSerial.h
	$(CC) -c wiringSerial.c wiringSerial.h

init.o: init.c init.h
	$(CC) -c init.c init.h

task1_mavdata.o: task1_mavdata.c task1_mavdata.h global.h
	$(CC) -c task1_mavdata.c task1_mavdata.h

task2_videodata.o: task2_videodata.c task2_videodata.h 
	$(CC) -c task2_videodata.c task2_videodata.h

task3_mavpddl.o: task3_mavpddl.c task3_mavpddl.h global.h
	$(CC) -c $^

clean:
	@echo "cleanning project"
	rm  -rf main *.o *.gch
	@echo "clean completed"
