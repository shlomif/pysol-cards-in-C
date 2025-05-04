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
# import time
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

    sysname = 'fedora:42'
    pull = client.images.pull(sysname)
    print(pull)
    image = client.images.get(sysname)
    # image = pull
    print(image)
    containers = client.containers
    # container = image.create()
    container = containers.create(image)
    print(container)
    # container.attach(eot=4)
    # container.attach()
    container2 = containers.run(image=image, detach=True,)
    print(container)
    print(container2)
    # time.sleep(5)
    ret = container.exec_run(
        cmd='echo helloworld\n',
        demux=True,
    )
    print(container)
    print(ret)
    # container.run()
    print('before exec_run', container)

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
