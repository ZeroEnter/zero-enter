import traceback

from xrpl.clients import JsonRpcClient, WebsocketClient
from xrpl.wallet import Wallet
from xrpl.models import Subscribe, StreamParameter

JSON_RPC_URL = "hooks-testnet-v3.xrpl-labs.com"
client = JsonRpcClient(JSON_RPC_URL)


class XRPLHooksListener:
    transfer_wallet = None

    def __init__(self):
        self.transfer_wallet = Wallet.from_seed("shpJBbnNVAgPbv7NLwvS3nUbeahR4")
        self.transfer_wallet_address = "rwY5SoDxyfUcUijRvFyvJLnGy5CYPU5amZ"

    def prepare_transaction(self):
        raise NotImplementedError

    def listen(self):
        url = "wss://hooks-testnet-v3.xrpl-labs.com/"
        req = Subscribe(streams=[StreamParameter.TRANSACTIONS])
        # NOTE: this code will run forever without a timeout, until the process is killed
        with WebsocketClient(url) as client:
            client.send(req)
            for message in client:
                print(message)
                if message.get('transaction', {}).get('Destination', '') == self.transfer_wallet_address:
                    try:
                        self.prepare_transaction()
                    except:
                        traceback.print_exc()
