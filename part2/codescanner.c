#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "pmparser.h" // Your provided library for parsing /proc/self/maps

static int syscall_count = 0;

// Function to scan all executable memory regions for syscall instructions
void scan_executable_regions() {
    // Initialize a procmaps iterator to parse memory regions
    procmaps_iterator *maps_it = malloc(sizeof(procmaps_iterator));
    if (!maps_it || pmparser_parse(getpid(), maps_it) != PROCMAPS_SUCCESS) {
        fprintf(stderr, "Failed to parse memory maps.\n");
        return;
    }

    printf("Scanning for syscall instructions...\n");
    procmaps_struct *region;

    // Iterate over all memory regions
    while ((region = pmparser_next(maps_it)) != NULL) {
        // Skip non-executable regions
        if (!region->is_x) {
            continue;
        }

        // Print information about the current region
        printf("Scanning executable region: %p-%p\n", region->addr_start, region->addr_end);

        unsigned char *start = (unsigned char *)region->addr_start;
        unsigned char *end = (unsigned char *)region->addr_end;

        // Ensure the region is readable and executable
        if (mprotect(region->addr_start, region->length, PROT_READ | PROT_EXEC) == -1) {
            continue; // Skip if permissions cannot be set
        }

        // Scan for the syscall instruction (0x0F 0x05)
        for (unsigned char *addr = start; addr < end - 1; ++addr) {
            if ((addr + 1) < end && *addr == 0x0F && *(addr + 1) == 0x05) {
                printf("Syscall found at address: %p in region: %p-%p\n",
                       (void *)addr, region->addr_start, region->addr_end);
                       syscall_count++;
            }
        }

        // Restore original permissions (optional, for safety)
        mprotect(region->addr_start, region->length, PROT_READ | PROT_WRITE | PROT_EXEC);
    }

    // Free the iterator and its resources
    pmparser_free(maps_it);
    free(maps_it);
}

// Constructor function that initializes the scanner when the library is loaded
__attribute__((constructor))
void initialize_scanner() {
    printf("Initialize Scanning...\n");
    scan_executable_regions();
    printf("Total numbe of system calls found: %d\n", syscall_count);
    printf("Scanning Completed\n");
    exit(1); // Exit after scanning to prevent further execution of the target application
}