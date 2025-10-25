\# C Web Crawler



A simple, lightweight C-language web crawler designed to fetch webpages, save them locally, and respect specified depth limits. It avoids fetching duplicate URLs and restricts its crawling to internal links only.



\## Features



\- \*\*URL De-duplication:\*\* Avoids re-fetching and re-processing the same URL.  

\- \*\*Local Storage:\*\* Saves fetched HTML content to a specified local directory.  

\- \*\*Depth Control:\*\* Stops crawling once a maximum specified depth is reached.  

\- \*\*Internal Links Only:\*\* Ensures the crawler stays within the domain of the seed URL.  



\## Requirements



You need a C compiler (like gcc) and `make` installed on your system.



\## Build



To compile the source files and create the executable, run:

```bash

make



The executable requires three arguments in the following order:

```bash

./crawler <seed\_URL> <save\_directory> <max\_depth>



Example:

./crawler https://engineering.ucsc.edu/ crawler\_dir 3



To remove compiled files:

```bash

make clean



Note: Be careful with high maxDepth values; large websites can use a lot of disk space.

