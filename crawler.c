#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "crawler.h"
#include "curl.h"
#include "pagedir.h"
#include <string.h>
#include "url.h"
#include <ctype.h> 

webpage_t* webpage_new(char *seedURL, int depth) { //Creates new webpage
  webpage_t* webpage = (webpage_t *) malloc(sizeof(webpage_t)); 
  if (webpage == NULL) {
    return NULL;
  }
  webpage->url = strdup(seedURL);
  webpage->html = NULL; 
  webpage->length = 0; 
  webpage->depth = depth; 
  return webpage;
}


void webpage_delete(webpage_t *webpage) { //Frees memory of webpages
    if (webpage != NULL) {
        free(webpage->url);
        free(webpage->html);
        free(webpage);
    }
}

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) { //Gets args
	if (argc == 4) {
		if (0 <= atoi(argv[3]) && atoi(argv[3]) <= 10 && pagedir_init(argv[2])) {
			*seedURL = argv[1];
			*pageDirectory = argv[2];
        		*maxDepth = atoi(argv[3]);
        		return;
        	}
	} 
	printf("Error\n");
	exit(1);
}

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
	int docID = 1; 
	webpage_t *webPage = webpage_new(seedURL,0);
	hashtable_t *hashtable1 = hashtable_new(maxDepth);
	hashtable_insert(hashtable1, seedURL, "");
	bag_t *bag1 = set_new();
	set_insert(bag1, webPage, 0);
	node* currentNode = bag1->head;
    	while (currentNode != NULL) { //Goes through the bag and saves webpages into directory
		webpage_t* pulled = currentNode->key;
		if(pagefetcher(pulled)) { //Checks if html was successfully gotten
			pagedir_save(pulled, pageDirectory, docID);
			docID++;
			if (pulled->depth!=maxDepth) { //Scans if the current depth is less than max depth 	
				printf("%d %s: %s\n", pulled->depth, "Scanning", pulled->url);
				pageScan(pulled, bag1, hashtable1);
			}	
		}
		currentNode=currentNode->next;
		webpage_delete(pulled);
	}   
	hashtable_delete(hashtable1,NULL);
	set_delete(bag1, NULL);
}

bool pagefetcher(webpage_t *page) { //Gets html
	size_t htmlSize = 0;
	page->html = download(page->url, &htmlSize);
	printf("%d %s: %s\n", page->depth, "Fetched", page->url);
	page->length = (long unsigned int) htmlSize;
	if (page->html!=NULL) {
		return true;
	}
	return false;
}

/**
 * Scans a webpage for URLs.
 */

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) { 
	char *urlOrg = page->html;
    	while ((urlOrg = strstr(urlOrg, "<a")) != NULL) { //Finds first instance of <a
        	char *href = strstr(urlOrg, "href"); //checks if href is contained in urlOrg
        	if (href) { //parses past href and finds url
        	    href += 4; 
        	    while (*href && !(*href == '\"' || *href == '\'')) { 
        	        href++;
        	    }
           	 if (*href) { //adds actual url into hashtable, creates new webpage and puts the webpage in the bag/set
                	char *actualURL = href + 1;
                	char *urlEnd = strchr(actualURL, *href);
	                if (urlEnd != NULL) {
	                    *urlEnd = '\0';
	                    if (strchr(actualURL, '#') == NULL) {
	                        char *extractedURL = normalizeURL(page->url, actualURL);
	                        printf("%d %s: %s\n", page->depth, "Found", extractedURL);
	                        if (extractedURL != NULL) {
	                            if (isInternalURL(page->url, extractedURL)) {
	                                if (hashtable_insert(pagesSeen, extractedURL, "")) {
	                                    webpage_t *webPage2 = webpage_new(extractedURL, page->depth + 1);
	                                    set_insert(pagesToCrawl, webPage2, 0);
	                                    printf("%d %s: %s\n", page->depth, "added", extractedURL);
	                                }
	                                else {
	                                    printf("%d %s: %s\n", page->depth, "IgnDupl", extractedURL);
	                                }
	                            }
	                            else{
	                                printf("%d %s: %s\n", page->depth, "IgnExtrn", extractedURL);
	                            }
	                            free(extractedURL);
	                        }
	                    }
	                    urlOrg = urlEnd + 1;
	                } 
		else {
        		break;
                	}
            	} 
		else {
       			break;
            	}
        	} 
		else {
            		urlOrg += 2; 
        	}
    	}
}

int main(const int argc, char* argv[]) { 
    char* seedURL;
    char* pageDirectory;
    int maxDepth;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    exit(0);
}
