demo:
	touch output.sk
	gcc main.c -o skib
#cd test; tcc -run -b ../main.c main.sk ../output.sk
	cd test; ../skib main.sk ../output.sk
	@echo "----"
	@cat output.sk
	@echo "----"
	rm output.sk

clean:
	rm -rf skib *.out
