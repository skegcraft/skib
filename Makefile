skib: main.c
	$(CC) main.c -o skib

install: skib
	cp skib /bin/

clean:
	rm -rf skib *.out

demo: skib
	touch output.sk
	cd test; ../skib main.sk ../output.sk
	@echo "----"
	@cat output.sk
	@echo "----"
	rm output.sk

# Server live test
FILE?=~/Skegcraft/plugins/Skript/scripts/test.sk
SCREEN?=skegcraft
server:
	touch $(FILE)
	$(CC) main.c -o skib
	cd test; ../skib main.sk $(FILE)
	sudo screen -S $(SCREEN) -X stuff "skript reload test\n"
	sudo screen -S $(SCREEN) -X stuff "test\n"
