#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fnmatch.h>

typedef struct {
    char *pattern;
    long min_size;
    long max_size;
    int show_hidden;
    int file_count;
    int dir_count;
    long total_size;
} walker_config;

void print_detailed_info(const char *filepath, struct stat *file_stat, walker_config *config) {
    if (!config->show_hidden && filepath[strlen(filepath)-1] == '.' && 
        strrchr(filepath, '/') && strrchr(filepath, '/')[1] == '.') {
        return;
    }
    
    if (config->pattern && fnmatch(config->pattern, filepath, 0) != 0) {
        return;
    }
    
    if (file_stat->st_size < config->min_size || 
        (config->max_size > 0 && file_stat->st_size > config->max_size)) {
        return;
    }
    
    printf("Path: %s\n", filepath);
    printf("  Size: %ld bytes\n", file_stat->st_size);
    printf("  Permissions: ");
    
    printf((S_ISDIR(file_stat->st_mode)) ? "d" : "-");
    printf((file_stat->st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat->st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat->st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat->st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat->st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat->st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat->st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat->st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat->st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    
    struct passwd *pw = getpwuid(file_stat->st_uid);
    struct group *gr = getgrgid(file_stat->st_gid);
    printf("  Owner: %s (%d)\n", pw ? pw->pw_name : "unknown", file_stat->st_uid);
    printf("  Group: %s (%d)\n", gr ? gr->gr_name : "unknown", file_stat->st_gid);
    
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
             localtime(&file_stat->st_mtime));
    printf("  Last Modified: %s\n", time_str);
    
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
             localtime(&file_stat->st_atime));
    printf("  Last Accessed: %s\n", time_str);
    
    printf("  Inode: %ld\n", file_stat->st_ino);
    printf("  Links: %ld\n", file_stat->st_nlink);
    
    if (S_ISDIR(file_stat->st_mode)) {
        printf("  Type: Directory\n");
        config->dir_count++;
    } else {
        printf("  Type: Regular File\n");
        config->file_count++;
        config->total_size += file_stat->st_size;
    }
    
    printf("---\n");
}

int enhanced_walk(const char *path, int max_depth, int current_depth, walker_config *config) {
    if (current_depth > max_depth) {
        return 0;
    }
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
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
        
        if (lstat(full_path, &file_stat) == 0) {
            print_detailed_info(full_path, &file_stat, config);
            
            if (S_ISDIR(file_stat.st_mode)) {
                enhanced_walk(full_path, max_depth, current_depth + 1, config);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

void print_usage(const char *program_name) {
    printf("Usage: %s [path] [options]\n", program_name);
    printf("Options:\n");
    printf("  -d <depth>     Maximum depth (default: 2)\n");
    printf("  -p <pattern>   File pattern to match\n");
    printf("  -s <size>      Minimum file size in bytes\n");
    printf("  -S <size>      Maximum file size in bytes\n");
    printf("  -a             Show hidden files\n");
    printf("  -h             Show this help\n");
}

int main(int argc, char *argv[]) {
    walker_config config = {0};
    char *start_path = ".";
    int max_depth = 2;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            max_depth = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config.pattern = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            config.min_size = atol(argv[++i]);
        } else if (strcmp(argv[i], "-S") == 0 && i + 1 < argc) {
            config.max_size = atol(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            config.show_hidden = 1;
        } else if (argv[i][0] != '-') {
            start_path = argv[i];
        }
    }
    
    printf("Enhanced File System Walker\n");
    printf("Starting path: %s\n", start_path);
    printf("Maximum depth: %d\n", max_depth);
    if (config.pattern) printf("Pattern filter: %s\n", config.pattern);
    if (config.min_size > 0) printf("Minimum size: %ld bytes\n", config.min_size);
    if (config.max_size > 0) printf("Maximum size: %ld bytes\n", config.max_size);
    printf("Show hidden files: %s\n", config.show_hidden ? "Yes" : "No");
    printf("=====================================\n");
    
    enhanced_walk(start_path, max_depth, 0, &config);
    
    printf("\nSummary:\n");
    printf("Files found: %d\n", config.file_count);
    printf("Directories found: %d\n", config.dir_count);
    printf("Total file size: %ld bytes\n", config.total_size);
    
    return 0;
}
