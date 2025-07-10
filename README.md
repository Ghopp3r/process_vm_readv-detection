# Trap‑Page PoC — Detecting `process_vm_readv` / `process_vm_writev`

*A tiny, self‑contained demo that shows how a Linux/Android application can
detect when another process reads its memory via the
`process_vm_readv` / `process_vm_writev` syscalls.*

---

## 1 Idea in a Nutshell

1. Allocate one **anonymous**, **read‑only** page:

   ```cpp
   void* trap = mmap(nullptr, PAGE_SIZE,
                     PROT_READ,
                     MAP_PRIVATE | MAP_ANONYMOUS,
                     -1, 0);      // no physical frame yet
   ```
2. Replace a public pointer (e.g. `Engine::playerList`) with the address
   of that page.
3. As long as nobody touches the page, bit 63 (*present*) in
   `/proc/self/pagemap` is `0`.
4. When an external tool calls `process_vm_readv` and dereferences the
   pointer, the kernel resolves the page fault → `present` flips to `1`.
5. The application periodically checks the bit and knows immediately that
   its memory was inspected.

The helper that performs steps 1 and 5 lives in
`src/detector.{h,cpp}` and is <30 LoC.

---

## 2 Repository Layout

```
.
├── CMakeLists.txt
├── build.bat / build.sh          # one‑click build scripts
└── src
    ├── detector.h
    ├── detector.cpp              # trap::allocate / trap::touched
    ├── demo.cpp                  # victim game stub
    └── cheat.cpp                 # minimal reader (the “cheat”)
```

**Binaries produced**

| Target                | Purpose                          |
|-----------------------|----------------------------------|
| `detector_demo`       | Victim app with a fake player list. |
| `cheat_simulator`     | Reads an arbitrary address & length. |

---

## 3 Building

### 3.1 Desktop (Linux / WSL / macOS)

```bash
./build.sh            # outputs to ./build/
```

### 3.2 Android (NDK r25c+ on Windows)

```bat
build.bat             :: builds arm64‑v8a, x86_64, armeabi‑v7a
```

Resulting binaries land in `out/<ABI>/`.

---

## 4 Running the Demo

### 4.1 Generic Flow (desktop)

```bash
# Terminal 1 – victim
./detector_demo
# ↳ prints:  PID  and  trap_addr, then waits for <Enter>

# Terminal 2 – cheat  (needs root privileges)
sudo ./cheat_simulator <PID> <trap_addr> 16

# Back to Terminal 1: press <Enter>
# Output:  page backed? 1   ← intrusion detected
```

### 4.2 Android Phone (rooted)

```bash
adb push out/arm64-v8a/detector_demo     /data/local/tmp/
adb push out/arm64-v8a/cheat_simulator   /data/local/tmp/

# Terminal 1
adb shell "su -c 'cd /data/local/tmp && ./detector_demo'"

# Terminal 2
adb shell "su -c '/data/local/tmp/cheat_simulator <PID> <trap_addr> 16'"

# Return to Terminal 1 → <Enter> → detection fires.
```

---

## 5 Taking It Further

* Plant multiple trap pages to learn **which** structures attract a cheat.
* Return bogus data through the trap to break ESP overlays.
* Combine with **seccomp** / `ptrace_scope` to hard‑block the syscall for
  extra defense‑in‑depth.

---
