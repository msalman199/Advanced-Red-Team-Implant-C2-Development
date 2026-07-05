#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
    int total_files;
    int total_dirs;
    long total_size;
    int executable_files;
    int readable_files;
    int writable_files;
    char largest_file[1024];
    long largest_size;
} fs_statistics;

void update_stats(const char *filepath, struct stat *file_stat, fs_statistics *stats) {
    if (S_ISDIR(file_stat->st_mode)) {
        stats->total_dirs++;
    } else {
        stats->total_files++;
        stats->total_size += file_stat->st_size;
        
        if (file_stat->st_size > stats->largest_size) {
            stats->largest_size = file_stat->st_size;
            strncpy(stats->largest_file, filepath, sizeof(stats->largest_file) - 1);
        }
    }
    
    if (file_stat->st_mode & S_IXUSR) stats->executable_files++;
    if (file_stat->st_mode & S_IRUSR) stats->readable_files++;
    if (file_stat->st_mode & S_IWUSR) stats->writable_files++;
}

int collect_stats(const char *path, int max_depth, int current_depth, fs_statistics *stats) {
    if (current_depth > max_depth) {
        return 0;
    }
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return -1;
    }
    
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (stat(full_path, &file_stat) == 0) {
            update_stats(full_path, &file_stat, stats);
            
            if (S_ISDIR(file_stat.st_mode)) {
                collect_stats(full_path, max_depth, current_depth + 1, stats);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

void print_statistics(fs_statistics *stats) {
    printf("\nFile System Statistics\n");
    printf("======================\n");
    printf("Total Files: %d\n", stats->total_files);
    printf("Total Directories: %d\n", stats->total_dirs);
    printf("Total Size: %ld bytes (%.2f KB)\n", stats->total_size, stats->total_size / 1024.0);
    printf("Executable Files: %d\n", stats->executable_files);
    printf("Readable Files: %d\n", stats->readable_files);
    printf("Writable Files: %d\n", stats->writable_files);
    
    if (stats->largest_size > 0) {
        printf("Largest File: %s (%ld bytes)\n", stats->largest_file, stats->largest_size);
    }
    
    if (stats->total_files > 0) {
        printf("Average File Size: %.2f bytes\n", (double)stats->total_size / stats->total_files);
    }
}

int main(int argc, char *argv[]) {
    char *start_path = ".";
    int max_depth = 5;
    
    if (argc > 1) {
        start_path = argv[1];
    }
    if (argc > 2) {
        max_depth = atoi(argv[2]);
    }
    
    fs_statistics stats = {0};
    
    printf("Collecting file system statistics from: %s\n", start_path);
    printf("Maximum depth: %d\n", max_depth);
    
    if (collect_stats(start_path, max_depth, 0, &stats) == 0) {
        print_statistics(&stats);
    } else {
        printf("Error collecting statistics\n");
        return 1;
    }
    
    return 0;
}
