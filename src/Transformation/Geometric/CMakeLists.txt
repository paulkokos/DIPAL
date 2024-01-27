project(Geometric)

set(GEOMETRIC_SOURCES GeometricTransformation.cpp
        Reflection.cpp
        Rotation.cpp
        Scaling.cpp
        Shear.cpp
        Translation.cpp) # Setup the list of sources here.
add_library( Geometric ${GEOMETRIC_SOURCES} )
# You can refer to other libraries which should be available from the root cmakelists.
#target_link_libraries( lib3 lib1 lib2 )