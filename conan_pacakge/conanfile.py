from conans import ConanFile, CMake, tools
import os

class Common(ConanFile):
    name = "Common"
    version = "0.1"
    license = '(c) JoyStream Inc. 2016-2017'
    url = "https://github.com/JoyStream/common-conan.git"
    source_url = "git@github.com:JoyStream/common-cpp.git"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "CoinCore/0.1@joystream/stable"

    def source(self):
        self.run("git clone %s" % self.source_url)
        self.run("cd common-cpp && git checkout master")

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake common-cpp %s' % (cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="common-cpp/include/")
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["common"]

        if str(self.settings.compiler) != "Visual Studio":
            self.cpp_info.cppflags.append("-std=c++11")
