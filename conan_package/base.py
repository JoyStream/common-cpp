from conans import ConanFile, CMake, tools
import os

class CommonBase(ConanFile):
    name = "Common"
    version = "0.2.0" # should match the git tag without the 'v' preifx
    url = "https://github.com/JoyStream/common-cpp.git"
    repo_https_url = "https://github.com/JoyStream/common-cpp.git"
    repo_ssh_url ="git@github.com:JoyStream/common-cpp.git"
    license = '(c) JoyStream Inc. 2016-2017'

    build_policy = "missing"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "CoinCore/0.2.0@joystream/stable"

    def source(self):
        raise Exception("abstract base package was exported")

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake repo/sources %s' % (cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.hpp", dst="include", src="repo/sources/include/")
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["common"]

        if str(self.settings.compiler) != "Visual Studio":
            self.cpp_info.cppflags.append("-std=c++11")
