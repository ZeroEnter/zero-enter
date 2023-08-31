const express = require('express');
const xrpl = require("xrpl");
const crypto = require('crypto');
const elliptic = require('elliptic');
const {xrpToDrops} = require("xrpl");
const EC = elliptic.ec;

// Create and initialize EC context
const ec = new EC('secp256k1');
const app = express();
app.use(express.json());

const XRPL_WEBSOCKET_URL = "wss://hooks-testnet-v3.xrpl-labs.com"; // Testnet server
const MY_WALLET_SEED = 'shpJBbnNVAgPbv7NLwvS3nUbeahR4'; // Replace with your wallet's seed

const client = new xrpl.Client(XRPL_WEBSOCKET_URL);
const myWallet = xrpl.Wallet.fromSeed(MY_WALLET_SEED);
const senderWallet = xrpl.Wallet.fromSeed('snJ349hXNS8f1TVpgBJAd9HEwib9a')
app.post('/send-payment', async (req, res) => {
    const { destination, amount, payload } = req.body;

    if (!destination || !amount) {
        return res.status(400).send({ message: 'Destination and amount are required.' });
    }

    try {
        // Import the private and public keys
        const privateKey = ec.keyFromPrivate(myWallet.privateKey, 'hex');

// Example payload to sign
        const msgHash = crypto.createHash('sha256').update(payload).digest();

// Sign the message hash with your private key
        const signedPayload = privateKey.sign(msgHash).toDER('hex');

        const tx = await client.autofill({
            TransactionType: 'Payment',
            Account: senderWallet.address,
            Amount:  xrpl.xrpToDrops(100),
            Destination: destination,
            NetworkID: 21338,
            Fee: xrpl.xrpToDrops(1),
            Memos: [
                {
                    Memo:{
                        MemoType: xrpl.convertStringToHex(string="proof-signature"),
                        MemoData: xrpl.convertStringToHex(string=msgHash),
                        MemoFormat: xrpl.convertStringToHex(string='signed/payload+1'),
                    }
                },
                {
                    Memo:{
                        MemoType: xrpl.convertStringToHex(string="proof-signature"),
                        MemoData: xrpl.convertStringToHex(string=signedPayload),
                        MemoFormat: xrpl.convertStringToHex(string='signed/signature+1'),
                    }
                },
                {
                    Memo:{
                        MemoType: xrpl.convertStringToHex(string="proof-signature"),
                        MemoData: xrpl.convertStringToHex(string=myWallet.publicKey),
                        MemoFormat: xrpl.convertStringToHex(string='signed/publickey+1'),
                    }
                }
            ]


        });
        const signedTx = senderWallet.sign(tx);
        const txResult = await client.submitAndWait(tx, {
            autofill: true, // Adds in fields that can be automatically set like fee and last_ledger_sequence
            wallet: senderWallet
        });

        res.send({ txResult });
    } catch (error) {
        res.status(500).send({ message: error.message });
    }
});

const PORT = 3000;
app.listen(PORT, () => {
    client.connect().then(result => {
        console.log(result);
    }).catch(error => {
        console.error(error);
    });
    console.log(`Server started on http://localhost:${PORT}`);
});
