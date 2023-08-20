import os
import shutil

import click


@click.command()
@click.option(
    "--vk_path", default="proof/vk.key", help="VK Path"
)
@click.option(
    "--proof_path", default="proof/proof.key", help="Client seed"
)
def generate_hook(vk_path, proof_path):
    current_directory = os.path.dirname(os.path.abspath(__file__))
    build_folder: str = "build"
    os.makedirs(build_folder, exist_ok=True)
    hooks_src = f'{build_folder}/hooks-src'
    os.makedirs(hooks_src, exist_ok=True)
    shutil.copytree(f'{current_directory}/hooks', hooks_src, dirs_exist_ok=True)


if __name__ == "__main__":
    generate_hook()
