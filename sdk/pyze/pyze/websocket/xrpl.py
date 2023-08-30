import json
import traceback

import requests
import xrpl
from xrpl.clients import JsonRpcClient, WebsocketClient
from xrpl.transaction import submit_and_wait
from xrpl.utils import xrp_to_drops
from xrpl.wallet import Wallet
from xrpl.models import Subscribe, StreamParameter, Payment

JSON_RPC_URL = "wss://hooks-testnet-v3.xrpl-labs.com"


class XRPLHooksListener:
    transfer_wallet = None

    def __init__(self):
        self.transfer_wallet_address = "rwY5SoDxyfUcUijRvFyvJLnGy5CYPU5amZ"

    def prepare_transaction(self, client, message):
        proof_link = ""
        for memo in message['transaction']['Memos']:
            memo_type = bytes.fromhex(memo['Memo']["MemoType"]).decode('utf-8')
            memo_data = bytes.fromhex(memo['Memo']["MemoData"]).decode('utf-8')
            if memo_type == 'proof-link':
                proof_link = memo_data

        response = requests.get(proof_link)
        data = json.loads(response.content)

        # Verify

        # Endpoint URL
        URL = "http://localhost:3000/send-payment"

        # Payment data
        data = {
            "destination": message['transaction']['Account'],  # Replace with the actual destination XRPL address
            "amount": "1",  # 1 XRP in drops
            "memos": [
                {
                    "data": "SomeData",
                    "type": "Description",
                    "format": "text/plain"
                },
                {
                    "data": "AnotherMemo",
                    "type": "Note",
                    "format": "text/markdown"
                }
            ]
        }

        response = requests.post(URL, json=data)

        # Print the response
        if response.status_code == 200:
            print("Transaction details:", response.json())
        else:
            print(f"Error ({response.status_code}):", response.text)

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
                        self.prepare_transaction(client, message)
                    except:
                        traceback.print_exc()
