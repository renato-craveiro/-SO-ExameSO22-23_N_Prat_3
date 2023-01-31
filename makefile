play:  play.o
	gcc play.o  -o play

play.o: play.c
	gcc -lpthread -c play.c


inst:  instrumento.o
	gcc instrumento.o -o inst

inst.o: instrumento.c
	gcc -c instrumento.c

clean:
	rm *.o 

uninstall:
	rm *.o play inst

intro:
	@echo "\n\n-----Sistemas Operativos: Ex. 3 Parte Prática (Exame Normal 2022/2023-----\n\nImplementação um bocado à "padeiro". Pfv. alterem/adaptem e se conseguirem melhor solução, pfv partilhem.\n\n"

all: intro play inst
