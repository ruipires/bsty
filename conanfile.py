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

        # https://github.com/gabime/spdlog
        self.requires("spdlog/1.8.0")

        # https://github.com/nlohmann/json
        self.requires("nlohmann_json/3.9.1")

        # header-only library for reading comma separated value (CSV) files
        # https://github.com/ben-strasser/fast-cpp-csv-parser
        #self.requires("fast-cpp-csv-parser/20191004")
        self.requires("fast-cpp-csv-parser/cci.20200830")

        # https://www.gaia-gis.it/fossil/freexl/index
        #self.requires("freexl/1.0.6")

        # lightweight C++ option parser library
        # https://github.com/jarro2783/cxxopts
        self.requires("cxxopts/2.2.1")

        # https://github.com/HowardHinnant/date
        self.requires("date/3.0.0")

        # https://unicode-org.github.io/icu/
        self.requires("icu/68.1")

        # Catch2 - unit testing framework
        # https://github.com/catchorg/Catch2
        self.requires("catch2/2.13.3")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()