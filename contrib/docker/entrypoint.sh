#!/bin/sh -e

case $1 in
  spacexpanse-tx)
    exec "$@"
    ;;

  spacexpanse-cli)
    shift
    exec spacexpanse-cli \
      --datadir="/var/lib/spacexpanse" \
      --rpcconnect="${HOST}" \
      --rpcpassword="${RPC_PASSWORD}" \
      "$@"
    ;;

  spacexpansed)
    bin=$1
    shift
    ;;

  *)
    bin=spacexpansed
    ;;
esac

if [ -z "${RPC_PASSWORD}" ]
then
  echo "RPC_PASSWORD must be set"
  exit 1
fi

exec $bin \
  --datadir="/var/lib/spacexpanse" \
  --rpcpassword="${RPC_PASSWORD}" \
  --rpcbind="${HOST}" \
  --rpcallowip="${RPC_ALLOW_IP}" \
  --zmqpubgameblocks="tcp://${HOST}:${ZMQ_PORT}" \
  --zmqpubgamepending="tcp://${HOST}:${ZMQ_PORT}" \
  "$@"
