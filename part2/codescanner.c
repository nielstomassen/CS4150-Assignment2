#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "pmparser.h" // Your provided library for parsing /proc/self/maps

void scan_executable_regions() {
    procmaps_iterator *maps_it = malloc(sizeof(procmaps_iterator));
    if (!maps_it || pmparser_parse(getpid(), maps_it) != PROCMAPS_SUCCESS) {
        fprintf(stderr, "Failed to parse memory maps.\n");
        return;
    }

    printf("Scanning for syscall instructions...\n");
    procmaps_struct *region;
    while ((region = pmparser_next(maps_it)) != NULL) {
        // Skip non-executable regions
        if (!region->is_x) {
            continue;
        }

        // Print the region being scanned
        printf("Scanning executable region: %p-%p\n", region->addr_start, region->addr_end);

        unsigned char *start = (unsigned char *)region->addr_start;
        unsigned char *end = (unsigned char *)region->addr_end;

        // Ensure region is accessible
        if (mprotect(region->addr_start, region->length, PROT_READ | PROT_EXEC) == -1) {
            continue; // Skip this region if mprotect fails
        }

        for (unsigned char *addr = start; addr < end - 1; ++addr) {
            // Check for syscall instruction
            if ((addr + 1) < end && *addr == 0x0F && *(addr + 1) == 0x05) {
                printf("Syscall found at address: %p in region: %p-%p\n",
                       (void *)addr, region->addr_start, region->addr_end);
            }
        }

        // Optional: Restore original permissions (if needed)
        mprotect(region->addr_start, region->length, PROT_READ | PROT_WRITE | PROT_EXEC);
    }

    pmparser_free(maps_it);
    free(maps_it);
}

__attribute__((constructor))
void initialize_scanner() {
    printf("Initialize Scanning...\n");
    scan_executable_regions();
    printf("Scanning Completed\n");
    exit(1);
}