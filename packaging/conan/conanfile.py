from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get
import os


class DipalConan(ConanFile):
    name = "dipal"
    version = "0.1.2"
    license = "MIT"
    author = "Pavlos Kokozidis <paulkokos@yahoo.gr>"
    url = "https://github.com/paulkokos/DIPAL"
    description = "Digital Image Processing and Analysis Library - A modern C++23 image processing library"
    topics = ("image-processing", "computer-vision", "cpp23")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        # Force static on Windows (DLL exports not implemented)
        if self.settings.os == "Windows":
            self.options.shared = False

    def layout(self):
        cmake_layout(self)

    def source(self):
        get(self, f"https://github.com/paulkokos/DIPAL/archive/refs/tags/v{self.version}.tar.gz",
            strip_root=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTS"] = False
        tc.variables["BUILD_EXAMPLES"] = False
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["dipal"]
        self.cpp_info.set_property("cmake_file_name", "DIPAL")
        self.cpp_info.set_property("cmake_target_name", "DIPAL::DIPAL")
