PROG=	ls
OBJS=	cal.o cmp.o print.o ls.o main.o
CFLAGS= -Wall 
LCFLAGS = -lcrypto -lbsd

all: ${PROG}

${PROG}: ${OBJS}
	@echo $@ depends on $?
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} -o ${PROG} ${LCFLAGS}

clean:
	rm -f lss *.o *~
