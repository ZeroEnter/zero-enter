import os
import shutil

import click

from pyze.websocket.xrpl import XRPLHooksListener


@click.command()
def listener():
    xrpl_listener= XRPLHooksListener()
    xrpl_listener.listen()


if __name__ == "__main__":
    listener()
