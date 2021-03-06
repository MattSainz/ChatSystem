#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=ChatCoordinator
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/chat_coordinator.o \
	${OBJECTDIR}/dict.o \
	${OBJECTDIR}/dict_string.o \
	${OBJECTDIR}/session_server.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk /Users/Matthias/Code/Git/ChatSystem/Build/chatcoordinator

/Users/Matthias/Code/Git/ChatSystem/Build/chatcoordinator: ${OBJECTFILES}
	${MKDIR} -p /Users/Matthias/Code/Git/ChatSystem/Build
	${LINK.c} -o /Users/Matthias/Code/Git/ChatSystem/Build/chatcoordinator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/chat_coordinator.o: nbproject/Makefile-${CND_CONF}.mk chat_coordinator.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/chat_coordinator.o chat_coordinator.c

${OBJECTDIR}/dict.o: nbproject/Makefile-${CND_CONF}.mk dict.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dict.o dict.c

${OBJECTDIR}/dict_string.o: nbproject/Makefile-${CND_CONF}.mk dict_string.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dict_string.o dict_string.c

${OBJECTDIR}/session_server.o: nbproject/Makefile-${CND_CONF}.mk session_server.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/session_server.o session_server.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/chat_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -lcunit 


${TESTDIR}/tests/chat_test.o: tests/chat_test.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -include chat_coordinator.h -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/chat_test.o tests/chat_test.c


${OBJECTDIR}/chat_coordinator_nomain.o: ${OBJECTDIR}/chat_coordinator.o chat_coordinator.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/chat_coordinator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/chat_coordinator_nomain.o chat_coordinator.c;\
	else  \
	    ${CP} ${OBJECTDIR}/chat_coordinator.o ${OBJECTDIR}/chat_coordinator_nomain.o;\
	fi

${OBJECTDIR}/dict_nomain.o: ${OBJECTDIR}/dict.o dict.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/dict.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dict_nomain.o dict.c;\
	else  \
	    ${CP} ${OBJECTDIR}/dict.o ${OBJECTDIR}/dict_nomain.o;\
	fi

${OBJECTDIR}/dict_string_nomain.o: ${OBJECTDIR}/dict_string.o dict_string.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/dict_string.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dict_string_nomain.o dict_string.c;\
	else  \
	    ${CP} ${OBJECTDIR}/dict_string.o ${OBJECTDIR}/dict_string_nomain.o;\
	fi

${OBJECTDIR}/session_server_nomain.o: ${OBJECTDIR}/session_server.o session_server.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/session_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/session_server_nomain.o session_server.c;\
	else  \
	    ${CP} ${OBJECTDIR}/session_server.o ${OBJECTDIR}/session_server_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} /Users/Matthias/Code/Git/ChatSystem/Build/chatcoordinator

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
