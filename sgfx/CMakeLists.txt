
if(CMSDK_IS_ARM)
    set(LIB_OPTION kernel)
    set(ARM_DEPENDENCIES StratifyOS_interface)
endif()

set(LIBRARY_NAME sgfx)

cmsdk2_add_library(
	NAME ${LIBRARY_NAME}
	CONFIG release
    OPTION "${LIB_OPTION}"
	ARCH ${CMSDK_ARCH}
    TARGET RELEASE_TARGET)

cmsdk2_add_sources(
    TARGET ${RELEASE_TARGET}
    DIRECTORY src)
    target_compile_options(${RELEASE_TARGET} PUBLIC ${API_PUBLIC_COMPILE_OPTIONS})

target_include_directories(${RELEASE_TARGET}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_options(${RELEASE_TARGET}
    PRIVATE
    -Wno-address-of-packed-member)

cmsdk2_add_library(
    TARGET DEBUG_TARGET
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION}"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${DEBUG_TARGET})

if(CMSDK_IS_ARM)
    set(LIB_OPTION_BPP kernel_)
endif()

cmsdk2_add_library(
    TARGET RELEASE_TARGET_1BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}1bpp"
    CONFIG release
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET RELEASE_TARGET_2BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}2bpp"
    CONFIG release
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET RELEASE_TARGET_4BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}4bpp"
    CONFIG release
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET RELEASE_TARGET_8BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}8bpp"
    CONFIG release
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET RELEASE_TARGET_16BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}16bpp"
    CONFIG release
    ARCH ${CMSDK_ARCH})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${RELEASE_TARGET_1BPP})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${RELEASE_TARGET_2BPP})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${RELEASE_TARGET_4BPP})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${RELEASE_TARGET_8BPP})

cmsdk2_copy_target(
    SOURCE ${RELEASE_TARGET}
    DESTINATION ${RELEASE_TARGET_16BPP})
    

cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET_1BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET_2BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")


cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET_4BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")


cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET_8BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")


cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET_16BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_add_library(
    TARGET DEBUG_TARGET_1BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}1bpp"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET DEBUG_TARGET_2BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}2bpp"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET DEBUG_TARGET_4BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}4bpp"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET DEBUG_TARGET_8BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}8bpp"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_add_library(
    TARGET DEBUG_TARGET_16BPP
    NAME ${LIBRARY_NAME}
    OPTION "${LIB_OPTION_BPP}16bpp"
    CONFIG debug
    ARCH ${CMSDK_ARCH})

cmsdk2_copy_target(
    SOURCE ${DEBUG_TARGET}
    DESTINATION ${DEBUG_TARGET_1BPP})

cmsdk2_copy_target(
    SOURCE ${DEBUG_TARGET}
    DESTINATION ${DEBUG_TARGET_2BPP})

cmsdk2_copy_target(
    SOURCE ${DEBUG_TARGET}
    DESTINATION ${DEBUG_TARGET_4BPP})

cmsdk2_copy_target(
    SOURCE ${DEBUG_TARGET}
    DESTINATION ${DEBUG_TARGET_8BPP})

cmsdk2_copy_target(
    SOURCE ${DEBUG_TARGET}
    DESTINATION ${DEBUG_TARGET_16BPP})
    

cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET_1BPP}    
    DEPENDENCIES "${ARM_DEPENDENCIES}")


cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET_2BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")


cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET_4BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET_8BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET_16BPP}
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_library_add_dependencies(
    TARGET ${RELEASE_TARGET}    
    DEPENDENCIES "${ARM_DEPENDENCIES}")

cmsdk2_library_add_dependencies(
    TARGET ${DEBUG_TARGET}
    DEPENDENCIES "${ARM_DEPENDENCIES}")




