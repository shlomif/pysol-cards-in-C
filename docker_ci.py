#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Licensed under the terms of the MIT license.

"""

"""

# import json
from pathlib import Path
import re
import subprocess
import sys


class DockerWrapper:
    """docstring for DockerWrapper"""
    def __init__(self, filename, image_os):
        self.fn = filename
        self.image_os = image_os

    def write_file(self):
        with open(self.fn, 'wt') as fh:
            assert re.search("\\A[A-Za-z0-9\\:_]+\\Z", self.image_os)
            fh.write("FROM {}\n".format(self.image_os))
            fh.write("RUN echo helloworld\n")
            fh.write("RUN expr 4 \\* 6\n")
            fh.write("COPY . /git\n")
            pkgs = [
                "clang",
                "cmake",
                "diffutils",
                "freecell-solver",
                "make",
                "perl-core",
                "perl-devel",
                "python3-devel",
                "python3-pip",
                "python3-pysol-cards",
            ]
            assert pkgs == sorted(pkgs)
            fh.write("RUN dnf -y install " + ' '.join(pkgs) + "\n")
            fh.write("RUN pip install --upgrade " + ' '.join(["pysol_cards"])
                     + "\n")
            fh.write("RUN set -e -x; cd /git ; gmake retest\n")

    def run(self):
        subprocess.run(["podman", "build", "--file", self.fn, ".",])


if False:
    d = DockerWrapper(filename="Dockerfile", image_os="fedora:42")
    d.write_file()
    d.run()

curdir = Path(".")
dockerfile_dir = curdir / "hello-world-docker-action"
dockerfile_dir.mkdir(exist_ok=True, parents=True)
dockerfile_fn = dockerfile_dir / "Dockerfile"
d = DockerWrapper(filename=dockerfile_fn, image_os="fedora:42")
d.write_file()
d.run()
subprocess.run(["git", "add", dockerfile_fn, ])

sys.exit(0)
