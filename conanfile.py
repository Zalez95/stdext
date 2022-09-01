from conans import ConanFile, CMake

class StdExtConan(ConanFile):
	name = "stdext"
	version = "v1.0"
	license = "MIT"
	author = "Zalez95"
	settings = "os", "compiler", "build_type", "arch"
	options = {"shared": [True, False], "fPIC": [True, False]}
	default_options = {"shared": False, "fPIC": True}
	generators = "cmake"

	def config_options(self):
		if self.settings.os == "Windows":
			del self.options.fPIC

	def export_sources(self):
		self.copy("*")

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake(self)
		cmake.install()

	def package_info(self):
		self.cpp_info.includedirs = ["include"]
		self.cpp_info.libdirs = ["lib"]
		self.cpp_info.libs = [self.name]
