demo:
	touch output.sk
	$(CC) main.c -o skib
	cd test; ../skib main.sk ../output.sk
	@echo "----"
	@cat output.sk
	@echo "----"
	rm output.sk

FILE?=~/Skegcraft/plugins/Skript/scripts/test.sk
SCREEN?=skegcraft
server:
	touch $(FILE)
	$(CC) main.c -o skib
	cd test; ../skib main.sk $(FILE)
	sudo screen -S $(SCREEN) -X stuff "skript reload test\n"

clean:
	rm -rf skib *.out
