#include "../include/messanger.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <client_name>\n", argv[0]);
        exit(1);
    }

    run_client(argv[1]);
    return 0;
}