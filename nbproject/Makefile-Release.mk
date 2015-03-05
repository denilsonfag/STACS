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
CND_PLATFORM=Cygwin_4.x-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/code/AcoMtspAlgorithm.o \
	${OBJECTDIR}/code/AcsAlgorithm.o \
	${OBJECTDIR}/code/Ant.o \
	${OBJECTDIR}/code/Colony.o \
	${OBJECTDIR}/code/DoubleMatrix.o \
	${OBJECTDIR}/code/EuclideanGraph.o \
	${OBJECTDIR}/code/IntList.o \
	${OBJECTDIR}/code/Intersection.o \
	${OBJECTDIR}/code/LocalSearch.o \
	${OBJECTDIR}/code/MtspInstance.o \
	${OBJECTDIR}/code/MtspNearestNeighbor.o \
	${OBJECTDIR}/code/MtspSolution.o \
	${OBJECTDIR}/code/Node.o \
	${OBJECTDIR}/code/Random.o \
	${OBJECTDIR}/code/RealData.o \
	${OBJECTDIR}/code/RealExperiment.o \
	${OBJECTDIR}/code/RealMatrix.o \
	${OBJECTDIR}/code/Route.o \
	${OBJECTDIR}/code/ServiceOrder.o \
	${OBJECTDIR}/code/StandardExperiment.o \
	${OBJECTDIR}/code/TacoAlgorithm.o \
	${OBJECTDIR}/code/Utilities.o \
	${OBJECTDIR}/code/WorkDay.o \
	${OBJECTDIR}/code/main.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/prototype.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/prototype.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/prototype ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/code/AcoMtspAlgorithm.o: code/AcoMtspAlgorithm.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/AcoMtspAlgorithm.o code/AcoMtspAlgorithm.cpp

${OBJECTDIR}/code/AcsAlgorithm.o: code/AcsAlgorithm.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/AcsAlgorithm.o code/AcsAlgorithm.cpp

${OBJECTDIR}/code/Ant.o: code/Ant.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Ant.o code/Ant.cpp

${OBJECTDIR}/code/Colony.o: code/Colony.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Colony.o code/Colony.cpp

${OBJECTDIR}/code/DoubleMatrix.o: code/DoubleMatrix.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/DoubleMatrix.o code/DoubleMatrix.cpp

${OBJECTDIR}/code/EuclideanGraph.o: code/EuclideanGraph.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/EuclideanGraph.o code/EuclideanGraph.cpp

${OBJECTDIR}/code/IntList.o: code/IntList.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/IntList.o code/IntList.cpp

${OBJECTDIR}/code/Intersection.o: code/Intersection.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Intersection.o code/Intersection.cpp

${OBJECTDIR}/code/LocalSearch.o: code/LocalSearch.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/LocalSearch.o code/LocalSearch.cpp

${OBJECTDIR}/code/MtspInstance.o: code/MtspInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/MtspInstance.o code/MtspInstance.cpp

${OBJECTDIR}/code/MtspNearestNeighbor.o: code/MtspNearestNeighbor.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/MtspNearestNeighbor.o code/MtspNearestNeighbor.cpp

${OBJECTDIR}/code/MtspSolution.o: code/MtspSolution.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/MtspSolution.o code/MtspSolution.cpp

${OBJECTDIR}/code/Node.o: code/Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Node.o code/Node.cpp

${OBJECTDIR}/code/Random.o: code/Random.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Random.o code/Random.cpp

${OBJECTDIR}/code/RealData.o: code/RealData.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/RealData.o code/RealData.cpp

${OBJECTDIR}/code/RealExperiment.o: code/RealExperiment.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/RealExperiment.o code/RealExperiment.cpp

${OBJECTDIR}/code/RealMatrix.o: code/RealMatrix.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/RealMatrix.o code/RealMatrix.cpp

${OBJECTDIR}/code/Route.o: code/Route.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Route.o code/Route.cpp

${OBJECTDIR}/code/ServiceOrder.o: code/ServiceOrder.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/ServiceOrder.o code/ServiceOrder.cpp

${OBJECTDIR}/code/StandardExperiment.o: code/StandardExperiment.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/StandardExperiment.o code/StandardExperiment.cpp

${OBJECTDIR}/code/TacoAlgorithm.o: code/TacoAlgorithm.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/TacoAlgorithm.o code/TacoAlgorithm.cpp

${OBJECTDIR}/code/Utilities.o: code/Utilities.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Utilities.o code/Utilities.cpp

${OBJECTDIR}/code/WorkDay.o: code/WorkDay.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/WorkDay.o code/WorkDay.cpp

${OBJECTDIR}/code/main.o: code/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/main.o code/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/prototype.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
