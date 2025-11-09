#!/bin/bash

SERVER=./server
CLIENT=./client

# Inicia o servidor em background e captura o PID
$SERVER &
SERVER_PID=$!
echo "Server started (PID: $SERVER_PID)"
sleep 1

echo "========== TEST 1: Basic message =========="
$CLIENT $SERVER_PID "Hello, 42!"
sleep 1

echo -e "\n========== TEST 2: Empty message =========="
$CLIENT $SERVER_PID ""
sleep 1

echo -e "\n========== TEST 3: Long message =========="
LONG_MSG=$(printf 'x%.0s' {1..20000})
$CLIENT $SERVER_PID "$LONG_MSG"
sleep 2

echo -e "\n========== TEST 4: Multiple messages =========="
for i in {1..5}; do
  $CLIENT $SERVER_PID "Message $i"
done
sleep 2

echo -e "\n========== TEST 5: Speed test =========="
time $CLIENT $SERVER_PID "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
sleep 1

echo -e "\nAll tests sent. Check your server output for correctness."

# Encerra o servidor
kill $SERVER_PID 2>/dev/null
