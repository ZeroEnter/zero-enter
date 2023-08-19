import click

# Define the network client
from xrpl.clients import JsonRpcClient
from xrpl.wallet import generate_faucet_wallet

JSON_RPC_URL = "https://s.altnet.rippletest.net:51234/"
client = JsonRpcClient(JSON_RPC_URL)


def print_balance(wallet):
    print(wallet)


@click.command()
@click.option('--admin_seed', default='sEdTAVMeryLEy5BnuXQZgiXvcGFJ1wR', help='Admin seed')
@click.option('--client_seed', default='sEd7pkfPRWPEsFxHaCfkVdx4SXp5ryt', help='Client seed')
def send_payment(admin_seed, client_seed):
    print(admin_seed, client_seed)


if __name__ == '__main__':
    send_payment()
