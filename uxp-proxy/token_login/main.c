#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

typedef int (*legacy_write_fn)(
    const char *pathname_for_ftok,
    const char *password_id,
    const char *password,
    int password_length,
    int permissions
);

typedef int (*legacy_read_fn)(
    const char *pathname_for_ftok,
    const char *password_id,
    char ** return_buff,
    int* return_buff_len
);

void print_help(const char *prog) {
    fprintf(stdout,
        "Usage:\n"
        "  %s -w     Write password from $UXP_TOKEN_PASS to shared memory\n"
        "  %s -r     Read login info from shared memory\n"
        "  %s -h     Show this help message\n",
        prog, prog, prog
    );
}

int main(int argc, char **argv) {
    if (argc != 2) {
        print_help(argv[0]);
        return 1;
    }

    const char *arg = argv[1];
    const char *lib_path = "/usr/share/uxp/lib/libpasswordstore.so";
    const char *pathname = "/";
    const char *id = "0";
    int permissions = 0600;

    void *handle = dlopen(lib_path, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        return 1;
    }

    legacy_write_fn write_fn = (legacy_write_fn)dlsym(handle, "LEGACY_passwordWrite");
    legacy_read_fn read_fn = (legacy_read_fn)dlsym(handle, "LEGACY_passwordRead");

    if (!write_fn || !read_fn) {
        fprintf(stderr, "Failed to find required functions: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    if (strcmp(arg, "-w") == 0) {
        const char *password = getenv("UXP_TOKEN_PASS");
        if (!password) {
            fprintf(stderr, "Environment variable UXP_TOKEN_PASS is not set\n");
            dlclose(handle);
            return 1;
        }

        int write_result = write_fn(pathname, id, password, strlen(password), permissions);
        //fprintf(stdout, "LEGACY_passwordWrite returned: %d\n", write_result);
        dlclose(handle);
        if (write_result != 0) {
            fprintf(stdout, "Write password failed\n");
            return 1;
        }
        fprintf(stdout, "Write password OK!\n");
        return 0;

    } else if (strcmp(arg, "-r") == 0) {
        char *return_buff = NULL;
        int return_buff_len = 0;
        int read_result = read_fn(pathname, id, &return_buff, &return_buff_len);
        //fprintf(stdout, "LEGACY_passwordRead returned len=%d\n", return_buff_len);

        if (return_buff) free(return_buff);
        dlclose(handle);

        if (read_result != 0) {
            fprintf(stderr, "Call Read token error!\n");
            return 1;
        }
        if (return_buff_len == 0) {
            fprintf(stderr, "Not logged in.\n");
            return 2;
        }

        fprintf(stdout, "Logged in!\n");
        return 0;

    } else {
        print_help(argv[0]);
        dlclose(handle);
        return 1;
    }
}