
cmake_minimum_required(VERSION 2.8)
 
PROJECT(DeformPointSet)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(DeformPointSet MACOSX_BUNDLE DeformPointSet.cxx )
 
target_link_libraries(DeformPointSet ${VTK_LIBRARIES})
