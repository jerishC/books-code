SET (ACE_INCLUDE_DIRS $ENV{ACE_ROOT})
SET (ACE_LIBRARY_DIRS $ENV{ACE_ROOT}/lib)

IF (UNIX)
	SET (ACE_LIBRARIES ACE dl)
	SET (ACE_SSL_LIBRARIES ACE_SSL ${ACE_LIBRARIES})

	SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE)
	SET (ACE_C_FLAGS "-fno-strict-aliasing -Wall -W -Wpointer-arith")

	INCLUDE (CheckCXXCompilerFlag)

	IF (CMAKE_COMPILER_IS_GNUCC)
		IF (CMAKE_C_COMPILER_VERSION VERSION_GREATER 4.2 OR CMAKE_C_COMPILER_VERSION VERSION_EQUAL 4.2)
			CHECK_CXX_COMPILER_FLAG (-fvisibility=hidden HAVE_HIDDEN_VISIBILITY)
			CHECK_CXX_COMPILER_FLAG (-fvisibility-inlines-hidden HAVE_INLINES_HIDDEN_VISIBILITY)
		ENDIF (CMAKE_C_COMPILER_VERSION VERSION_GREATER 4.2 OR CMAKE_C_COMPILER_VERSION VERSION_EQUAL 4.2)
	ENDIF (CMAKE_COMPILER_IS_GNUCC)

	CHECK_CXX_COMPILER_FLAG (-pthread HAVE_PTHREAD)

	IF (HAVE_HIDDEN_VISIBILITY)
		SET (ACE_C_FLAGS "${ACE_C_FLAGS} -fvisibility=hidden")
	ENDIF (HAVE_HIDDEN_VISIBILITY)

	IF (HAVE_INLINES_HIDDEN_VISIBILITY)
		SET (ACE_CXX_FLAGS "${ACE_CXX_FLAGS} -fvisibility-inlines-hidden")
	ENDIF (HAVE_INLINES_HIDDEN_VISIBILITY)

	IF (HAVE_PTHREAD)
		SET (ACE_C_FLAGS "${ACE_C_FLAGS} -pthread")
		SET (ACE_CXX_FLAGS "${ACE_CXX_FLAGS} -pthread")
	ENDIF (HAVE_PTHREAD)

	IF ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
		CHECK_CXX_COMPILER_FLAG (-Wl,-E HAVE_LD_EXPORT_DYNAMIC)

		IF (HAVE_PTHREAD)
			SET (ACE_LIBRARIES ${ACE_LIBRARIES} rt)
		ENDIF (HAVE_PTHREAD)

		IF (HAVE_LD_EXPORT_DYNAMIC)
			SET (ACE_LINK_FLAGS ${ACE_LINK_FLAGS} -Wl,-E)
		ENDIF (HAVE_LD_EXPORT_DYNAMIC)
	ENDIF ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
ELSEIF (WIN32)
	IF (MINGW)
		SET (ACE_LIBRARIES ACE ws2_32)
		SET (ACE_SSL_LIBRARIES ACE_SSL ${ACE_LIBRARIES})
	ELSE (MINGW)
		SET (ACE_LIBRARIES debug ACEd optimized ACE)
		SET (ACE_SSL_LIBRARIES debug ACE_SSLd optimized ACE_SSL ${ACE_LIBRARIES})
	ENDIF (MINGW)

	IF (MINGW)
		SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} -DACE_HAS_CUSTOM_EXPORT_MACROS=0)
	ELSE (MINGW)
		SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
	ENDIF (MINGW)

	IF (MSVC)
		# Disable warning C4706:
		#   assignment within conditional expression
		SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} /wd4706)
	ENDIF (MSVC)
ELSEIF (APPLE)
	MESSAGE ("Apple platform ...")
ENDIF (UNIX)

IF (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} -DACE_NO_INLINE)
ELSE(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	SET (ACE_DEFINITIONS ${ACE_DEFINITIONS} -DACE_NDEBUG -D__ACE_INLINE__)
ENDIF (${CMAKE_BUILD_TYPE} MATCHES "Debug")
