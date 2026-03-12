<div align="center">

# 📡 Minitalk

### *Sending messages one bit at a time*

<img src="https://img.shields.io/badge/42-SP-000000?style=for-the-badge&logo=42&logoColor=white"/>
<img src="https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white"/>
<img src="https://img.shields.io/badge/IPC-UNIX Signals-FF6B6B?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Bonus-Included-4ECDC4?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge"/>

</div>

---

## 📖 Table of Contents

- [About](#-about)
- [How It Works](#-how-it-works)
- [Project Structure](#-project-structure)
- [UNIX Signals](#-unix-signals)
- [Bit Encoding Protocol](#-bit-encoding-protocol)
- [Bonus Features](#-bonus-features)
- [Installation & Compilation](#-installation--compilation)
- [Usage](#-usage)
- [Test Cases](#-test-cases)
- [Custom Tester](#-custom-tester)
- [Allowed Functions](#-allowed-functions)
- [Author](#-author)

---

## 📚 About

**Minitalk** is a project from the **42 School** curriculum that introduces **Inter-Process Communication (IPC)** using exclusively **UNIX signals**. The goal is to build a small client-server messaging system where the client sends a string to the server, character by character, encoded as individual bits — using only two signals: `SIGUSR1` and `SIGUSR2`.

This project teaches how processes communicate asynchronously, how to handle signals reliably with `sigaction`, and how to encode/decode data at the binary level.

---

## ⚙️ How It Works

The communication between client and server follows a strict **bit-by-bit transmission protocol**:

```
CLIENT                                SERVER
  │                                     │
  │── reads PID of server ─────────────►│
  │                                     │
  │  for each character in the string:  │
  │    for each of the 8 bits:          │
  │── SIGUSR1 (bit = 0) ───────────────►│ accumulates bit
  │── SIGUSR2 (bit = 1) ───────────────►│ accumulates bit
  │                                     │ when 8 bits received → prints char
  │                                     │
  │  (bonus) ◄─── ACK signal ──────────│ confirms reception
  │                                     │
  │── sends '\0' (end of string) ──────►│ prints newline, resets state
```

### 🖥️ Server (`server.c`)

1. Prints its own **PID** on startup — the client needs this to know where to send signals.
2. Sets up signal handlers for `SIGUSR1` and `SIGUSR2` using `sigaction`.
3. Waits indefinitely using `pause()`, waking up only when a signal arrives.
4. Each received signal contributes one bit to the current character being reconstructed.
5. When **8 bits** are collected, the character is printed.
6. When a **null character (`\0`)** is received, the message is complete and the server resets.

### 📱 Client (`client.c`)

1. Takes the **server PID** and a **message string** as arguments.
2. Iterates over each character of the string (including the final `\0`).
3. For each character, sends **8 signals** — one per bit, LSB first:
   - `SIGUSR1` → bit is **0**
   - `SIGUSR2` → bit is **1**
4. Uses `usleep` between signals to give the server time to process each one.

---

## 🗂️ Project Structure

```
Minitalk/
│
├── minitalk.h            # Shared header — prototypes and includes
│
├── server.c              # Mandatory: signal handler + message reconstruction
├── client.c              # Mandatory: bit encoding + signal transmission
│
├── server_bonus.c        # Bonus: server with ACK acknowledgment to client
├── client_bonus.c        # Bonus: client that waits for ACK before next bit
│
├── utils.c               # Utility functions shared between programs
│
├── my_minitalk_tester.sh # Custom shell tester script
│
└── Makefile              # Builds server, client, and bonus versions
```

---

## 📶 UNIX Signals

Minitalk uses only two UNIX signals for all communication:

| Signal | Value | Meaning in Protocol |
|---|---|---|
| `SIGUSR1` | 10 | Represents bit **0** |
| `SIGUSR2` | 12 | Represents bit **1** |

### Why `sigaction` instead of `signal`?

The project uses `sigaction` for signal handling because it is **safer and more portable** than the older `signal()` function. `sigaction` provides:
- Access to sender's PID via `siginfo_t` (needed for the bonus ACK).
- More predictable behavior across different UNIX systems.
- Ability to block other signals while the handler runs.

```c
struct sigaction sa;
sa.sa_sigaction = handle_signal;
sa.sa_flags = SA_SIGINFO;
sigemptyset(&sa.sa_mask);
sigaction(SIGUSR1, &sa, NULL);
sigaction(SIGUSR2, &sa, NULL);
```

---

## 🔢 Bit Encoding Protocol

Each character is transmitted as **8 individual signals**, one per bit. The encoding works as follows:

```
Character: 'A' → ASCII 65 → Binary: 01000001

Bit 0 (LSB): 1 → send SIGUSR2
Bit 1:       0 → send SIGUSR1
Bit 2:       0 → send SIGUSR1
Bit 3:       0 → send SIGUSR1
Bit 4:       0 → send SIGUSR1
Bit 5:       0 → send SIGUSR1
Bit 6:       1 → send SIGUSR2
Bit 7 (MSB): 0 → send SIGUSR1
```

On the server side, bits are shifted into a `char` variable:

```c
// SIGUSR2 received (bit = 1)
current_char |= (1 << bit_index);
bit_index++;

// SIGUSR1 received (bit = 0)
bit_index++;  // bit stays 0 — no OR needed

// After 8 bits:
if (bit_index == 8)
{
    write(1, &current_char, 1);
    bit_index = 0;
    current_char = 0;
}
```

---

## ⭐ Bonus Features

The bonus version (`server_bonus.c` / `client_bonus.c`) adds a **handshake/acknowledgment system**:

- After receiving and processing each bit, the **server sends a signal back** to the client confirming reception.
- The **client waits** for this acknowledgment before sending the next bit.
- This eliminates timing issues — no more `usleep` guesswork.
- Supports sending **multiple messages** to the same running server without restarting.

```
CLIENT                    SERVER
  │── bit (SIGUSR1/2) ──►│
  │◄── ACK (SIGUSR1) ────│
  │── next bit ─────────►│
  │◄── ACK ──────────────│
  ...
```

---

## ⚙️ Installation & Compilation

### Prerequisites

- GCC or Clang
- GNU Make
- Linux or macOS (POSIX signals required)

### Clone & Build

```bash
git clone https://github.com/BrenoLSR/Minitalk.git
cd Minitalk
make
```

### Makefile Targets

| Target | Description |
|---|---|
| `make` / `make all` | Compiles `server` and `client` (mandatory) |
| `make bonus` | Compiles `server_bonus` and `client_bonus` |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and all binaries |
| `make re` | Full recompile from scratch |

---

## 🚀 Usage

### Step 1 — Start the server

```bash
./server
```

The server will print its **PID** immediately:

```
Server PID: 12345
```

### Step 2 — Send a message from the client

In a separate terminal:

```bash
./client <server_PID> "<message>"
```

**Example:**

```bash
./client 12345 "Hello, 42!"
```

The server will output:

```
Hello, 42!
```

### Bonus version

```bash
./server_bonus
# → Server PID: 12345

./client_bonus 12345 "Hello with ACK!"
```

---

## 🧪 Test Cases

| Command | Expected Behavior |
|---|---|
| `./client <PID> "Hello"` | Server prints `Hello` |
| `./client <PID> "Hello, World!"` | Server prints `Hello, World!` |
| `./client <PID> ""` | Server receives and handles empty string |
| `./client <PID> "42SP"` | Server prints `42SP` |
| `./client <PID> "Unicode: 🚀"` | Tests multi-byte/unicode handling |
| Send multiple messages in sequence | Server handles each without restarting |

### Edge Cases to Test

- Very long strings (1000+ characters)
- Strings with special characters and spaces
- Sending multiple messages to the same server instance
- Two clients sending to the same server (bonus handles this more gracefully)

---

## 🧰 Custom Tester

This project includes a custom shell tester **`my_minitalk_tester.sh`** written specifically to automate testing of the client-server communication.

```bash
# Make it executable
chmod +x my_minitalk_tester.sh

# Run the tester
./my_minitalk_tester.sh
```

The tester automatically launches the server, sends a series of test messages via the client, and validates the output — making it easy to catch regressions or timing issues quickly.

---

## 📋 Allowed Functions

| Function | Header | Purpose |
|---|---|---|
| `write` | `<unistd.h>` | Output to file descriptor |
| `ft_printf` | (libft) | Formatted output |
| `malloc` / `free` | `<stdlib.h>` | Dynamic memory |
| `signal` | `<signal.h>` | Basic signal handling (legacy) |
| `sigaction` | `<signal.h>` | Robust signal handling with `siginfo_t` |
| `sigemptyset` | `<signal.h>` | Initialize an empty signal set |
| `sigaddset` | `<signal.h>` | Add signal to a set |
| `kill` | `<signal.h>` | Send a signal to a process by PID |
| `getpid` | `<unistd.h>` | Get the current process ID |
| `pause` | `<unistd.h>` | Suspend until a signal is received |
| `sleep` | `<unistd.h>` | Sleep for N seconds |
| `usleep` | `<unistd.h>` | Sleep for N microseconds |
| `exit` | `<stdlib.h>` | Terminate the process |

---

## 👤 Author

<div align="center">

**Breno LSR**
42 São Paulo

[![GitHub](https://img.shields.io/badge/GitHub-BrenoLSR-181717?style=for-the-badge&logo=github)](https://github.com/BrenoLSR)
[![42](https://img.shields.io/badge/42-São_Paulo-000000?style=for-the-badge&logo=42&logoColor=white)](https://www.42sp.org.br/)

</div>

---

<div align="center">

*"In a world of bits and signals, every message finds its way."*

</div>
