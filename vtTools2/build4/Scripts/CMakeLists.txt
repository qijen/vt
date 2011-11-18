cmake_minimum_required(VERSION 2.6)

SET( CMAKE_CURRENT_SOURCE_DIR "./CMAKEFiles")
SET( CMAKE_HOST_WIN32 "TRUE")


#--------Looking for Dev 35 Directory ! Stored in variable "VTDEV35DIR

include ("CMakeFindMakeVTDev35.cmake")

IF(VTDEV35DIR)
	MESSAGE(STATUS "\n\n\t Virtools Dev Directory  3.5 found  : ${VTDEV35DIR} \n \n")
ELSE(VTDEV35DIR)
	MESSAGE(ERROR "\n\n\t Virtools Dev Directory  3.5 NOT found  !!!	\n \n")
ENDIF(VTDEV35DIR)
file(APPEND "VTPaths.lua" "DEV35DIR=\"${VTDEV35DIR}\"	\n" )






#--------Looking for Dev 40 Directory ! Stored in variable "VTDEV40DIR

include ("CMakeFindMakeVTDev40.cmake")

IF(VTDEV40DIR)
		MESSAGE(STATUS "\n\n\t Virtools Dev Directory  4.0 found  : ${VTDEV40DIR} \n \n")
ELSE(VTDEV40DIR)
	MESSAGE(ERROR "\n\n\t Virtools Dev Directory  4.0 NOT found  !!!	\n \n")
ENDIF(VTDEV40DIR)
file(APPEND "VTPaths.lua" "DEV40DIR=\"${VTDEV40DIR}\" \n" )


#--------Looking for Dev 41 Directory ! Stored in variable "VTDEV41DIR

include ("CMakeFindMakeVTDev41.cmake")

IF(VTDEV41DIR)
		MESSAGE(STATUS "\n\n\t Virtools Dev Directory  4.1 found  : ${VTDEV41DIR} \n \n")
ELSE(VTDEV41DIR)
	MESSAGE(ERROR "\n\n\t Virtools Dev Directory  4.1 NOT found  !!!	\n \n")
ENDIF(VTDEV41DIR)

file(APPEND "VTPaths.lua" "DEV41DIR=\"${VTDEV41DIR}\"	\n" )





