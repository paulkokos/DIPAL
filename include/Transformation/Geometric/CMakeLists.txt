project(Geometric)

set(GEOMETRIC_SOURCES GeometricTransformation.hpp GeometricTransformation.cpp
        Reflection.cpp Reflection.hpp
        Rotation.cpp Rotation.hpp
        Scaling.cpp Scaling.hpp
        Shear.cpp Shear.hpp
        Translation.cpp Translation.hpp) # Setup the list of sources here.
add_library( Geometric ${GEOMETRIC_SOURCES} )
# You can refer to other libraries which should be available from the root cmakelists.
#target_link_libraries( lib3 lib1 lib2 )