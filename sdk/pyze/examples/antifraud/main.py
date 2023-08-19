import click

# Define the network client
from xrpl.clients import JsonRpcClient
from xrpl.models import Payment
from xrpl.transaction import submit_and_wait
from xrpl.utils import xrp_to_drops
from xrpl.wallet import generate_faucet_wallet, Wallet

from pyze.memo import generate_verification_memos

JSON_RPC_URL = "https://s.altnet.rippletest.net:51234/"
client = JsonRpcClient(JSON_RPC_URL)


def get_ml_proof_memos():
    return generate_verification_memos()


@click.command()
@click.option('--wallet_address', default='rKhEk6HQMGHJi4KcqrjQqWjSCmwegyz4Rp',
              help='Admin seed')  # seed sEdTAVMeryLEy5BnuXQZgiXvcGFJ1wR
@click.option('--client_seed', default='sEd7pkfPRWPEsFxHaCfkVdx4SXp5ryt', help='Client seed')
def send_payment(wallet_address, client_seed):
    client_account = Wallet.from_seed(client_seed)

    my_tx_payment = Payment(
        account=client_account.classic_address,
        amount=xrp_to_drops(22),
        destination=wallet_address,
        memos=get_ml_proof_memos()
    )

    tx_response = submit_and_wait(my_tx_payment, client, client_account)
    print(tx_response)


if __name__ == '__main__':
    send_payment()
