#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "globals.h"
#include "lexer_interface.h"
#include "parser_interface.h"

char *input_filename = NULL;

int main(int argc, char **argv){
    static struct option long_options[] = {
        {"tracescan", no_argument, (int *) &traceScan, true},
        {"scanfile", required_argument, NULL, 's'},
        {"traceparse", no_argument, (int *) &traceParse, true},
        {"parsefile", required_argument, NULL, 'p'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "s:p:o:", long_options, &option_index);
    while (c != -1){
        switch (c){
            case 0:
                break;
            case 's':
                scan_file = fopen(optarg, "w");
                if (scan_file == NULL){
                    fprintf(stderr, "Could not open scan file %s.\n", optarg);
                    exit(1);
                }
                traceScan = true;
                break;
            case 'o':
                // This will be moved when the next phase is implemented
            case 'p':
                parse_file = fopen(optarg, "w");
                if (parse_file == NULL){
                    fprintf(stderr, "Could not open parse file %s\n", optarg);
                    exit(1);
                }
                traceParse = true;
                break;
            case '?':
                // getopt_long already printed an error message
                exit(1);
                break;
            default:
                abort();
        }
        c = getopt_long(argc, argv, "s:p:o:", long_options, &option_index);
    }

    FILE *source;

    if (optind < argc){
        input_filename = strdup(argv[optind]);
        source = fopen(input_filename, "r");
        if (source == NULL){
            fprintf(stderr, "Could not open input file %s\n", input_filename);
            fclose(parse_file);
            fclose(scan_file);
            exit(1);
        }
    } else {
        input_filename = strdup("stdin");
        source = stdin;
    }

    setSourceFile(source);

    traceParse = true; // Only when the parser is the last phase!

    if (traceScan && scan_file == NULL){
        scan_file = stdout;
    }

    if (traceParse && parse_file == NULL){
        parse_file = stdout;
    }

    ParseTree *parseTree = parse();

    // Here we would do something with the parse tree in the next phase

    freeTree(parseTree);

    return 0;
}
