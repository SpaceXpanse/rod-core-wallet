# SpaceXpanse Specs 

Name: SpaceXpanse
Ticker: ROD
Symbol: Ɍ
Address Letter: R
Address Header: 75
P2SH Header: 80
Port: 11998
RPC Port: 11999
Algorithm: Neoscrypt/SHA256D, for merged mining - chainid: 1899
Block Reward: 8 ROD¹, halving every 1054080 blocks /every year/ till the end of the fourth year when 5% inflation can be implemented to double the coin supply every ~15 years /TBC/.
Reward distribution: 75%/25% - 3 Neoscrypt, 1 SHA256D every 4 blocks till the begining of the third year when masternodes can be introduced /TBC/.
Block Time: 30 seconds
Spend: 6 confirmations
Block: 120 confirmations
Current Supply: ~199,999,998
Max. Supply: 999,999,999²
ICO/IEO: None
Premine: 199,999,998 coins in the developer's multisig 3/4 address and 2/3 of them will be transfered to time-locked addresses to be unlocked in the next four years³. The other will be used to support the first year of development.

¹There is a pre-release period of 86400 blocks /~30 days/ in the beginning, in which the reward will be 1 ROD.
²This will change if 5% inflation will be implemented.
³They will be inaccessible untill the target block numbers are reached. The unused coins from the premine will be burned accordingly.

This repository contains technical specifications and design documents that
describe how the various components and layers in the ecosystem interact.
In particular, important topics are:

* SpaceXpanse's [*triple-purpose mining*](mining.md) that secures the blockchain
* The basic SpaceXpanse [blockchain consensus protocol](blockchain.md)
* How [games](games.md) should interact with the core blockchain
* A standard for [currencies](currencies.md) on the SpaceXpanse platform
* In-game trading of assets for ROD using [atomic name updates](trading.md)
* The core daemon [interface](interface.md) for game engines

You can access SpaceXpanse tutorials here > https://github.com/Spacepanse/Documentation/wiki
