class Dipal < Formula
  desc "Digital Image Processing and Analysis Library - A modern C++23 image processing library"
  homepage "https://github.com/paulkokos/DIPAL"
  url "https://github.com/paulkokos/DIPAL/archive/refs/tags/v0.1.2.tar.gz"
  sha256 "UPDATE_WITH_ACTUAL_SHA256"
  license "MIT"
  head "https://github.com/paulkokos/DIPAL.git", branch: "master"

  depends_on "cmake" => :build
  depends_on "ninja" => :build
  depends_on "llvm@18" => :build

  def install
    ENV["CC"] = Formula["llvm@18"].opt_bin/"clang"
    ENV["CXX"] = Formula["llvm@18"].opt_bin/"clang++"

    system "cmake", "-S", ".", "-B", "build", "-G", "Ninja",
                    "-DCMAKE_CXX_STANDARD=23",
                    "-DBUILD_TESTS=OFF",
                    "-DBUILD_EXAMPLES=OFF",
                    *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    (testpath/"test.cpp").write <<~EOS
      #include <DIPAL/Image/GrayscaleImage.hpp>
      int main() {
        DIPAL::GrayscaleImage img(10, 10);
        return img.isEmpty() ? 1 : 0;
      }
    EOS
    system ENV.cxx, "-std=c++23", "test.cpp", "-I#{include}", "-L#{lib}", "-ldipal", "-o", "test"
    system "./test"
  end
end
