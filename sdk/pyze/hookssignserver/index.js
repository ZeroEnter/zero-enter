const express = require('express');
const xrpl = require("xrpl");

const app = express();
app.use(express.json());

const XRPL_WEBSOCKET_URL = "wss://hooks-testnet-v3.xrpl-labs.com"; // Testnet server
const MY_WALLET_SEED = 'shpJBbnNVAgPbv7NLwvS3nUbeahR4'; // Replace with your wallet's seed

const client = new xrpl.Client(XRPL_WEBSOCKET_URL);
const myWallet = xrpl.Wallet.fromSeed(MY_WALLET_SEED);
app.post('/send-payment', async (req, res) => {
    const { destination, amount, memos } = req.body;

    if (!destination || !amount) {
        return res.status(400).send({ message: 'Destination and amount are required.' });
    }

    try {

        const tx = await client.autofill({
            TransactionType: 'Payment',
            Account: myWallet.address,
            Amount:  xrpl.xrpToDrops(amount),
            Destination: destination,
            NetworkID: 21338,
            Memos: memos ? memos.map(memo => ({
                Memo: {
                    MemoData: memo.data ? Buffer.from(memo.data, 'utf8').toString('hex') : undefined,
                    MemoType: memo.type ? Buffer.from(memo.type, 'utf8').toString('hex') : undefined,
                    MemoFormat: memo.format ? Buffer.from(memo.format, 'utf8').toString('hex') : undefined
                }
            })) : undefined

        });
        const signedTx = myWallet.sign(tx);
        const txResult = await client.submitAndWait(tx, {
            autofill: true, // Adds in fields that can be automatically set like fee and last_ledger_sequence
            wallet: myWallet
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
