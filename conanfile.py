from conans import ConanFile, CMake


class BstyConan(ConanFile):
    name = "bsty"
    version = "0.1"
    license = "MIT"
    author = "Rui Pires rui@sennin.pt"
    url = "github.com/ruipires/bsty"
    description = "Bank Statements To YNAB (converter)"
    topics = ("bank", "finance", "importer", "converter")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def requirements(self):
        self.requires("spdlog/1.8.0")  # https://github.com/gabime/spdlog
        self.requires("nlohmann_json/3.9.1")  # https://github.com/nlohmann/json
        self.requires("fast-cpp-csv-parser/20191004")  # https://github.com/ben-strasser/fast-cpp-csv-parser
        self.requires("freexl/1.0.6")  # https://www.gaia-gis.it/fossil/freexl/index
        self.requires("cxxopts/2.2.1")  # https://github.com/jarro2783/cxxopts
        self.requires("date/3.0.0") # https://github.com/HowardHinnant/date
        self.requires("icu/68.1") # https://unicode-org.github.io/icu/

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()