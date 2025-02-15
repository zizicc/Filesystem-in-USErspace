# Filesystem in user space

1. A new file "hello" with string "Hello World!" is introduced under the root directory.
2. A new directory “time” is introduced under the root directory.
3. A new file “now.txt” is present within the “time” directory.
4. The “now.txt” will, when read, contain a 20-byte timestamp showing the current local time.
5. The file size of “now.txt” will be 20.
6. The modification time (mtime) of “now.txt” will be the current time when checked via the
“ls -l” or “stat” commands.
7. The UID owner of “now.txt” will be the current user checking the file.

## Usage
• Can mount the hello filesystem with “./hellotime mountpoint”; will background and go silent.

• Can unmount with “fusermount -u mountpoint”.
