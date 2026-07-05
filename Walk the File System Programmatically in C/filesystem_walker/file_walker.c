#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_file_info(const char *filepath, struct stat *file_stat) {
    printf("File: %s\n", filepath);
    printf("  Size: %ld bytes\n", file_stat->st_size);
    printf("  Permissions: %o\n", file_stat->st_mode & 0777);
    
    struct passwd *pw = getpwuid(file_stat->st_uid);
    struct group *gr = getgrgid(file_stat->st_gid);
    printf("  Owner: %s\n", pw ? pw->pw_name : "unknown");
    printf("  Group: %s\n", gr ? gr->gr_name : "unknown");
    
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", 
             localtime(&file_stat->st_mtime));
    printf("  Modified: %s\n", time_str);
    printf("  Type: %s\n", S_ISDIR(file_stat->st_mode) ? "Directory" : "File");
    printf("---\n");
}

int walk_directory(const char *path, int max_depth, int current_depth) {
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
        
        if (stat(full_path, &file_stat) == 0) {
            print_file_info(full_path, &file_stat);
            
            if (S_ISDIR(file_stat.st_mode)) {
                walk_directory(full_path, max_depth, current_depth + 1);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    char *start_path = ".";
    int max_depth = 2;
    
    if (argc > 1) {
        start_path = argv[1];
    }
    if (argc > 2) {
        max_depth = atoi(argv[2]);
    }
    
    printf("Walking file system starting from: %s\n");
    printf("Maximum depth: %d\n");
    printf("=====================================\n");
    
    walk_directory(start_path, max_depth, 0);
    
    return 0;
}
