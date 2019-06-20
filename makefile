#==============================================================================
# FILE: makefile
#
# DESCRIPTION:
# make file to build brain corp coding sample
#
# AUTHORS:
#   Brooke Wallace <brookbot@yahoo.com> 2017/02
#
# Copyright 2017, Brooke Wallace, All rights reserved.
#==============================================================================
default: all

RELPATHTOROOT=../
MAKEFILETYPE=app
TARGETNAME=test

PACKAGENAME=braincorp_sample
MAJOR_VERSION=1
MINOR_VERSION=1

NEEDINC=
NEEDLIB=

C-SRC+=\
   test.c

TEXT-SRC=\
   README.md\
   LICENSE

MAKEFILE=makefile
#==============================================================================
# Build rules and vars
#==============================================================================
ALL_SRC= ${C-SRC} ${MAKEFILE} ${TEXT-SRC}

OBJS=$(C-SRC:%.c=%.o)

ifeq (${MAKEFILETYPE},app)
   TARGET=${TARGETNAME}
else
   $(error MAKEFILETYPE ${MAKEFILETYPE} is not supported)
endif

${TARGET}: ${OBJS}
	${CC} ${OBJS} -o ${TARGET}

all: ${TARGET}

check:
	./${TARGET}

clean:
	rm -f ${OBJS}
	rm -f ${TARGET}

PACKAGE_VERSION_NAME=${PACKAGENAME}_${MAJOR_VERSION}.${MINOR_VERSION}
TAR_PACKAGE=${PACKAGE_VERSION_NAME}.tgz

pkg: ${TAR_PACKAGE}

${TAR_PACKAGE}:
	tar -cvzf ${TAR_PACKAGE} --transform="s%%${PACKAGE_VERSION_NAME}/%" ${ALL_SRC} 

pkg_clean:
	-rm -f ${TAR_PACKAGE}

vars:
	@echo MAKEFILETYPE=${MAKEFILETYPE}
	@echo TARGET=${TARGET}
	@echo C-SRC=${C-SRC}
	@echo OBJS=${OBJS}




