import matplotlib
import sys
sys.path.insert(0, '../')
matplotlib.use('Agg')
# from PPRVizS.pprviz import *
# from PPRVizS.config import filelist
from scipy import sparse
import argparse
import networkx as nx
import numpy as np
import time

filelist = {
            0: "twego",
            1: "fbego",
            2: "wikiedit",
            3: "physic",
            4: "trust",
            5: "scinet",
            6: "amazon",
            7: "youtube",
            }

# global supernode_list, mapping_data, A, n
A=None
Gfull = None
D = None
supernode_list=dict()
mapping_data=dict()
PPRDeg = None
Xmds = None
r = None
super2id = dict()
id2super = []
component = 0
level = 0

def load_ppr(path, size):
    global PPRDeg
    with open(path+".src", "rb") as fin:
        row = np.fromfile(fin, dtype=np.int32)
    with open(path+".dst", "rb") as fin:
        col = np.fromfile(fin, dtype=np.int32)
    with open(path+".ppr", "rb") as fin:
        data = np.fromfile(fin, dtype=np.double)
    assert (len(row)==len(col))
    assert (len(row)==len(data))
    # print (len(row),len(col))
    row1 = [super2id[i] for i in row]
    col1 = [super2id[i] for i in col]
    PPRDeg = sparse.csr_matrix((data, (row1, col1)), shape=(size, size))

def load_position(path):
    print(path)
    global Xmds, r
    with open(path+"_fpsn.x", "rb") as fin:
        x = np.fromfile(fin, dtype=np.double)
    with open(path+"_fpsn.y", "rb") as fin:
        y = np.fromfile(fin, dtype=np.double)
    with open(path+"_fpsn.r", "rb") as fin:
        r = np.fromfile(fin, dtype=np.double)
    # print (len(row),len(col))
    Xmds = np.vstack([x,y])
    Xmds = Xmds.T

def load_community(hiefname, mapfname):
    global supernode_list, mapping_data
    with open(hiefname, "r") as fin:
        while True:
            l = fin.readline()
            if len(l) == 0:
                break
            name = l.rstrip("\r\n")
            l = fin.readline()
            size = int(l.rstrip("\r\n"))
            child = [int(fin.readline().rstrip("\r\n")) for i in range(size)]
            supernode_list[name] = child

    with open(mapfname, "r") as fin:
        while True:
            l = fin.readline()
            if len(l) == 0:
                break
            name = l.rstrip("\r\n")
            l = fin.readline()
            size = int(l.rstrip("\r\n"))
            child = [int(fin.readline().rstrip("\r\n")) for i in range(size)]
            mapping_data[name] = child


def split_supernode(name):
    global component, level
    lst = name.split("_")
    component = int(lst[0][1:])
    level = int(lst[1][1:])
    sid = int(lst[2])
    return sid

def get_children(node):
    global super2id,id2super
    split_supernode(node)
    if level == 1:
        childsize = len(mapping_data[node])
        id2super = mapping_data[node]
    else:
        childsize = len(supernode_list[node])
        id2super = supernode_list[node]
    super2id = {id2super[i]: i for i in range(childsize)}

    return childsize

def get_subgraph(cluster):
    s = len(cluster)
    cm1 = np.zeros((s, n))
    nodeweight = [1] * s

    for i in range(s):
        supernode = cluster[i]
        if supernode in mapping_data:
            cidx = mapping_data[supernode]
            nodeweight[i] = len(cidx)
        else:
            cidx = [supernode]
            nodeweight[i] = 1
        cm1[i, :] = np.sum(A[cidx, :], axis=0)

    As = np.zeros((s, s))
    for i in range(s):
        supernode = cluster[i]
        if supernode in mapping_data:
            cidx = mapping_data[supernode]
        else:
            cidx = [supernode]
        As[:, i] = np.sum(cm1[:, cidx], axis=1)

    subG = nx.from_scipy_sparse_matrix(sparse.csr_matrix(As))

    return subG, nodeweight


def viz(target):
    start = time.time()
    if level==1:
        cluster = id2super
    else:
        cluster = ["c"+str(component)+"_l"+str(level-1)+"_"+str(i) for i in id2super]

    G, nodeweight = get_subgraph(cluster=cluster)
    edges = G.edges()
    t = time.time() - start
    print ("subgraph time:{}s n:{} m:{}".format(t,G.number_of_nodes(),G.number_of_edges()))

    start = time.time()

    t = time.time() - start
    print ("zoom-in time:{}s".format(t))
    edgelist = list(edges)
    
    with open(f'{target}-edge-list.txt', 'w+') as f:
        for edge in edgelist:
            f.write(str(edge[0]))
            f.write(" ")
            f.write(str(edge[1]))
            f.write("\n")
        
        f.close()

    # plot(pos=Xmds, edges=edges, radius = r,
    #     name="../pprvizl_output/{}-{}".format(filelist[dataid], target))



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process...')
    parser.add_argument('--supernode', type=str, default="c0_l2_57", help='Super node to zoom into')
    parser.add_argument('--file', type=str, default="./dataset.txt", help='FIle path to data set')
    args = parser.parse_args()
    super_node = args.supernode
    filename = args.file
    # global supernode_list, mapping_data, A, n
    # dataid = raw_input('data id :')
    dataid = 7
    # dataid = int(dataid.encode("utf-8"))
    dataname = filelist[dataid]
    hiefname = './hiename.dat' # %dataname # hard code this one, next week tuesday we refactor and make it generalized
    mapfname = './mapname.dat' # %dataname

    print("loading clusters... for" + super_node)
    load_community(hiefname, mapfname)
    print("loading edges...")
    # path = "/home/zhangsq/gviz-ppr/dataset/" + dataname +".txt"
    fpath = filename
    print(fpath)
    Gfull = nx.read_edgelist(fpath, nodetype=int)
    A = nx.adjacency_matrix(Gfull,nodelist=sorted(Gfull.nodes()))
    n = Gfull.number_of_nodes()


    storename = "./inputds250_25".format(dataname, dataname)
    zoompath = [[super_node]]

    for path in zoompath:
        for i in range(len(path)):
            each = path[i]
            childsize = get_children(each)
            pprpath = storename+each
            load_position(pprpath)
            if i+1<len(path):
                child = path[i+1]
            else:
                child = None
            viz(each)

