from conans import ConanFile, CMake


class BstyConan(ConanFile):
    name = "bsty"
    version = "0.1"
    license = "MIT"
    author = "Rui Pires rui@sennin.pt"
    url = "github.com/ruipires/bsty"
    description = "Bank Statements To YNAB (converter)"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def requirements(self):
        self.requires("spdlog/1.8.0")
        self.requires("nlohmann_json/3.9.1")
        self.requires("fast-cpp-csv-parser/20191004")
        self.requires("freexl/1.0.6")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="hello")
        cmake.build()