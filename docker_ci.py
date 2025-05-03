#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Licensed under the terms of the MIT license.

"""

"""

import json
from podman import PodmanClient

"""Demonstrate PodmanClient."""

# Provide a URI path for the libpod service.  In libpod, the URI can be a unix
# domain socket(UDS) or TCP.  The TCP connection has not been implemented in
# this package yet.

uri = "unix:///run/user/1000/podman/podman.sock"

with PodmanClient(base_url=uri) as client:
    version = client.version()
    if False:
        print("Release: ", version["Version"])
        print("Compatible API: ", version["ApiVersion"])
        print("Podman API: ",
              version["Components"][0]["Details"]["APIVersion"], "\n")

        # get all images
        for image in client.images.list():
            print(image, image.id, "\n")

    image = client.images.pull('fedora:42')
    print(image)
    containers = client.containers
    container = containers.create(image)
    print(container)
    container = containers.run(image=image,)
    print(container)
    container.exec_run(['bash', '-c', 'echo helloworld', ])

    if False:
        # find all containers
        for container in client.containers.list():
            # After a list call you would probably want to reload the container
            # to get the information about the variables such as status.
            # Note that list() ignores the sparse option and assumes True
            # by default.
            container.reload()
            print(container, container.id, "\n")
            print(container, container.status, "\n")

            # available fields
            print(sorted(container.attrs.keys()))

        print(json.dumps(client.df(), indent=4))
