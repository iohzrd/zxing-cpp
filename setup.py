import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
import setuptools
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from setuptools.command.test import test as TestCommand


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CatchTestCommand(TestCommand):
    """
    A custom test runner to execute both Python unittest tests and C++ Catch-
    lib tests.
    """

    def distutils_dir_name(self, dname):
        """Returns the name of a distutils build directory"""
        dir_name = "{dirname}.{platform}-{version[0]}.{version[1]}"
        return dir_name.format(
            dirname=dname, platform=sysconfig.get_platform(), version=sys.version_info
        )

    def run(self):
        # Run Python tests
        super(CatchTestCommand, self).run()
        print("\nPython tests complete, now running C++ tests...\n")
        # Run catch tests
        subprocess.call(
            ["./*_test"],
            cwd=os.path.join("build", self.distutils_dir_name("temp")),
            shell=True,
        )


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: "
                + ", ".join(e.name for e in self.extensions)
            )

        if platform.system() == "Windows":
            cmake_version = LooseVersion(
                re.search(r"version\s*([\d.]+)", out.decode()).group(1)
            )
            if cmake_version < "3.1.0":
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        # extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        extdir = os.path.abspath(self.get_ext_fullpath(ext.name))
        cmake_args = ["-DENABLE_PYBIND:BOOL=true"]

        cfg = "Debug" if self.debug else "Release"
        build_args = ["--config", cfg]

        if platform.system() == "Windows":
            cmake_args += [
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)
            ]
            if sys.maxsize > 2 ** 32:
                cmake_args += ["-A", "x64"]
            build_args += ["--", "/m"]
        else:
            cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg]
            build_args += ["--", "-j4"]

        env = os.environ.copy()
        env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get("CXXFLAGS", ""), self.distribution.get_version()
        )
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )
        print()  # Add an empty line for cleaner output


with open("README.md", "r") as fh:
    long_description = fh.read()

setup(
    name="ZXingCore",
    version="1.0.3",
    author="iohzrd",
    author_email="iohzrd@protonmail.com",
    description="Native(C++) version of ZXing bound to python using pybind11",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/iohzrd/zxing-cpp",
    packages=setuptools.find_packages(),
    # add extension module
    ext_modules=[CMakeExtension("ZXingCore")],
    # add custom build_ext command
    cmdclass=dict(build_ext=CMakeBuild, test=CatchTestCommand),
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Apache License",
        "Operating System :: OS Independent",
    ],
)