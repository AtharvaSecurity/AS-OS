# 🖥️ AS-OS v0.1

> A simple x86 operating system with GUI, mouse support, and KDE Plasma-like interface built from scratch.

![OS](https://img.shields.io/badge/OS-AS--OS-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![C++](https://img.shields.io/badge/C++-17-blue)
![Assembly](https://img.shields.io/badge/Assembly-x86-red)
![Build](https://img.shields.io/badge/build-passing-brightgreen)

## 📋 Table of Contents

* Features
* Quick Start
* Login Credentials
* Keyboard Shortcuts
* Terminal Commands
* Project Structure
* Building from Source
* Debugging
* Roadmap
* Contributing
* License
* Author

---

## ✨ Features

### Core System

* 🖥️ KDE Plasma–style graphical desktop
* 🖱️ Mouse support
* 🔐 Multi-user login system
* 📁 FAT-like file system
* 🔄 Basic multitasking
* 💾 Memory management

### Built-in Applications

| Application        | Description            |
| ------------------ | ---------------------- |
| 🌐 Chrome Browser  | Simulated web browser  |
| 💻 Terminal        | Command line interface |
| 📝 Kate Editor     | Text editor            |
| 📂 File Manager    | Browse files           |
| 🌍 Network Center  | Network status         |
| 📦 App Store       | Package manager        |
| ⚙️ System Settings | OS settings            |
| 🧮 Calculator      | Calculator             |

---

# 🚀 Quick Start

## Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y g++-multilib nasm grub-pc-bin qemu-system-x86 xorriso build-essential
```

## Build

```bash
git clone https://github.com/AtharvaSecurity/AS-OS.git
cd AS-OS

make clean
make run
```

### Available Commands

| Command      | Description         |
| ------------ | ------------------- |
| `make`       | Build kernel        |
| `make clean` | Remove build files  |
| `make iso`   | Create bootable ISO |
| `make run`   | Build & start QEMU  |

---

# 🔐 Login Credentials

| Username | Password | Role  |
| -------- | -------- | ----- |
| root     | asos2024 | ROOT  |
| atharva  | password | ADMIN |
| admin    | admin123 | ADMIN |
| guest    | guest    | USER  |

---

# ⌨️ Keyboard Shortcuts

| Key | Action       |
| --- | ------------ |
| 1   | Chrome       |
| 2   | Terminal     |
| 3   | Kate         |
| 4   | File Manager |
| 5   | Network      |
| 6   | App Store    |
| 7   | Settings     |
| 8   | Calculator   |
| 0   | Logout       |
| q   | Shutdown     |

---

# 💻 Terminal Commands

## Basic

```text
help
clear
exit
whoami
date
time
```

## File System

```text
ls
cd
pwd
```

## Memory

```text
ps
mem
kill
```

## Network

```text
net
ping
```

## Packages

```text
pkg
install
search
```

## Info

```text
neofetch
calc
```

---

# 📁 Project Structure

```text
AS-OS/
├── arch/
├── drivers/
├── include/
├── kernel/
├── iso/
├── Makefile
├── linker.ld
├── grub.cfg
└── README.md
```

---

# 🛠️ Build Steps

```bash
make clean
make
make iso
make run
```

---

# 🐛 Debugging

```bash
qemu-system-i386 -cdrom build/as-os.iso -s -S

gdb build/kernel.bin
target remote localhost:1234
break kernel_main
continue
```

---

# 📦 Packages

* Terminal
* Kate
* Chrome
* File Manager
* Calculator
* Settings
* Network
* Games
* Music
* Paint

---

# 🎯 Roadmap

## Completed

* Kernel
* GUI
* Mouse
* Keyboard
* Login System
* File System
* Network Simulation
* Package Manager

## Planned

* USB Support
* Audio
* Real Networking
* 64-bit
* Multi-core
* VESA Graphics
* Hardware Acceleration
* Security Features

---

# 🤝 Contributing

```bash
git checkout -b feature-name
git commit -m "Added feature"
git push origin feature-name
```

Then open a Pull Request.

---

# 📝 License

MIT License

---

# 👨‍💻 Author

**Atharva Mani Tripathi**

GitHub: https://github.com/AtharvaSecurity

Email: [atharvamanitripathitripathi@gmail.com](mailto:atharvamanitripathitripathi@gmail.com)

---

# 🙏 Credits

* OSDev Wiki
* KDE Plasma
* GNU GCC
* QEMU
* GRUB
* NASM

---

# ❓ FAQ

### Change users

Edit:

```text
drivers/auth.cpp
```

### Add terminal commands

Edit:

```text
kernel/kernel.cpp
```

### Change wallpaper

Edit:

```text
desktop()
```

### Add an application

Create a new function inside:

```text
kernel/kernel.cpp
```

---

# 🔗 Useful Links

* https://github.com/AtharvaSecurity/AS-OS
* https://wiki.osdev.org
* https://www.gnu.org/software/grub/manual/
* https://www.nasm.us/doc/

---

<div align="center">

Made with ❤️ using C++, Assembly, GRUB and QEMU

</div>
