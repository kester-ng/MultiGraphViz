
#include "graph.h"
#include "main_convert.h"

using namespace std;

char *infile   = NULL;
char *outfile  = NULL;
char *outfile_w  = NULL;
inline int type       = UNWEIGHTED;
bool do_renumber = false;

void
inline usage(char *prog_name, const char *more) {
  cerr << more;
  cerr << "usage: " << prog_name << " -i input_file -o outfile [-r] [-w outfile_weight]" << endl << endl;
  cerr << "read the graph and convert it to binary format." << endl;
  cerr << "-r\tnodes are renumbered from 0 to nb_nodes-1 (the order is kept)." << endl;
  cerr << "-w filename\tread the graph as a weighted one and writes the weights in a separate file." << endl;
  cerr << "-h\tshow this usage message." << endl;
  exit(0);
}

void
inline parse_args(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    if(argv[i][0] == '-') {
      switch(argv[i][1]) {
      case 'i':
    if (i==argc-1)
      usage(argv[0], "Infile missing really\n");
    infile = argv[i+1];
    i++;
    break;
      case 'o':
    if (i==argc-1)
      usage(argv[0], "Outfile missing\n");
        outfile = argv[i+1];
    i++;
    break;
      case 'w' :
    type = WEIGHTED;
        outfile_w = argv[i+1];
    i++;
    break;
      case 'r' :
    do_renumber=true;
    break;
      default:
    usage(argv[0], "Unknown option\n");
      }
    } else {
      // usage(argv[0], "More than one filename\n");
    }
  }
  if (infile==NULL)
      usage(argv[0], "In file is missing\n");

  if (outfile==NULL)
      usage(argv[0], "Out file is missing\n");
}

int
convert_edgelist_to_binary(int argc, char **argv) {
  parse_args(argc, argv);

  Graph g(infile, type);
  g.clean(type);

  if (do_renumber)
    g.renumber(type);

  g.display_binary(outfile, outfile_w, type);
  return 0;
}
