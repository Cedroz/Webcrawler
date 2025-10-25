# C Web Crawler

A simple, lightweight C-language web crawler designed for **educational and archival purposes**. It is distributed under the **MIT License**, which includes a strong disclaimer of warranty and liability. It's designed to fetch webpages, save them locally, and respect specified depth limits. It avoids fetching duplicate URLs and restricts its crawling to internal links only.

Note: Be careful with high maxDepth values; large websites can use a lot of disk space.

---

## Features

- **URL De-duplication:** Avoids re-fetching and re-processing the same URL.

- **Local Storage:** Saves fetched HTML content to a specified local directory.

- **Depth Control:** Stops crawling once a maximum specified depth is reached.

- **Internal Links Only:** Ensures the crawler stays within the domain of the seed URL.

## Requirements

You need a C compiler (like gcc) and `make` installed on your system.

## Build

To compile the source files and create the executable, run:

```bash
make
```

## Usage
The executable requires three arguments in the following order:

```bash
./crawler <seed_URL> <save_directory> <max_depth>
```

## Example
Example:
```bash
./crawler https://engineering.ucsc.edu/ crawler_dir 3
```
To remove compiled files:

## Cleanup
```bash
make clean
```