from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import cmake_layout, CMake, CMakeToolchain

class StdExtConan(ConanFile):
	name = "stdext"
	version = "v1.0.0"
	license = "MIT"
	author = "Zalez95"
	settings = "os", "compiler", "build_type", "arch"
	options = {"shared": [True, False], "fPIC": [True, False]}
	default_options = {"shared": False, "fPIC": True}

	def config_options(self):
		if self.settings.os == "Windows":
			del self.options.fPIC

	def layout(self):
		cmake_layout(self)

	def export_sources(self):
		copy(self, "LICENSE", self.recipe_folder, self.export_sources_folder)
		copy(self, "*.txt", self.recipe_folder, self.export_sources_folder)
		copy(self, "*.cpp", self.recipe_folder, self.export_sources_folder)
		copy(self, "*.h", self.recipe_folder, self.export_sources_folder)
		copy(self, "*.hpp", self.recipe_folder, self.export_sources_folder)

	def generate(self):
		tc = CMakeToolchain(self)
		tc.generate()

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake(self)
		cmake.install()

	def package_info(self):
		self.cpp_info.libs = ["stdext"]
