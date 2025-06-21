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
import re
import subprocess
import sys


class DockerWrapper:
    """docstring for DockerWrapper"""
    def __init__(self, image_os):
        self.image_os = image_os
        self.fn = "Dockerfile"

    def run(self):
        with open(self.fn, 'wt') as fh:
            assert re.search("\\A[A-Za-z0-9\\:_]+\\Z", self.image_os)
            fh.write("FROM {}\n".format(self.image_os))
            fh.write("RUN echo helloworld\n")
            fh.write("RUN expr 4 \\* 6\n")
            fh.write("COPY . /git\n")
            fh.write("RUN dnf -y install clang make python3-devel\n")
            fh.write("RUN set -e -x; cd /git ; gmake retest\n")
        subprocess.run(["podman", "build", ".",])


d = DockerWrapper(image_os="fedora:42")
d.run()

sys.exit(0)
