#include "lib.h"
#include <cmath>

void parameter(int argc, char** argv, unordered_map<string, string> &map){
    for(int i=1;i<argc;i+=2){
        map.insert({argv[i], argv[i+1]});
    }
}

void param_config(string &alg){
    if(alg=="powiter"){ isPowerIter =1;}
    else if(alg=="fora"){ isRWIdx = 1;}
    else if(alg=="foratp"){ isRWIdx=1; isFORASN = 1;}
    else if(alg == "fpsn"){ isFPSN=1;isBPSN=0;}
    else if(alg == "taupush"){ isFPSN=1;isBPSN=1;}
    else{exit(-1);}
}

double** make_2d_array(int r, int c)
{
    double** arr = new double*[r];
    for(int i = 0; i < r; i++)
        arr[i] = new double[c];

    return arr;
}

vector<vector<double>> dnppr(int argc, char *argv[]) {
    unordered_map<string, string> param;
    parameter(argc, argv, param);
    int fileno;
    int buildflag;
    int sample;
    double alpha;
    int random_query;
    int full_mode;
    int k;

    fileno = stoi(param.count("-f")?param["-f"]:"2");
    buildflag = stoi(param.count("-build")?param["-build"]:"0");
    verboses = stoi(param.count("-verbose")?param["-verbose"]:"0");
    alpha = stof(param.count("-a")?param["-a"]:"0.2");
    k = stoi(param.count("-k")?param["-k"]:"25");
    sample = stoi(param.count("-sample")?param["-sample"]:"1");
    thread_nums = 1; // multi-threading is not support currently
    alg = param.count("-alg")?param["-alg"]:"taupush";
    random_query = stoi(param.count("-random")?param["-random"]:"1");
    embed_on = stoi(param.count("-embed")?param["-embed"]:"1");
    full_mode = stoi(param.count("-full")?param["-full"]:"0");
    param_config(alg);
    int seed = stoi(param.count("-seed")?param["-seed"]:"2");
    string path_input = param.count("-path")?param["-path"]: "c0_l2_838";
    string input_path = param.count("-input")?param["-input"] : "/home/kester/youtube.txt";
    srand(seed);
    string datapath = "/home/kester/";

//    if (full_mode){
//        vector<int> ks = {25,50,100,500,1000};
//        for (int k:ks){
//            datapath = "../dataset/rand_"+to_string(k);
//            graph = Graph(datapath,alpha);
//            full_visualize();
//        }
//        return 0;
//    }

    // if (verboses)
       // cout << "dataset: "<<filelist[fileno]<<endl;

    hiename = std::string("hiename.dat");
    rootname = std::string("rootname.root");
    mapname = std::string("mapname.dat");
    storepath = std::string("inputds250") + "_" + to_string(k);


    graph = Graph(input_path,alpha,k);
    int max_level = load_multilevel();
    graph.max_level = max_level;

    prpath = "input.dnpr";
    if ((!buildflag && isFPSN) or (isBPSN)){
        prpath = "input.dnpr";
        build_dnpr();
        deserialize_pr();
    }

    if (buildflag){
        if (isBPSN)
            rwpath = "output";
        if (isRWIdx)
            rwpath = std::string("randwalks")+"_"+to_string(k);

//        int threads[] = {64,32,16,8,4,2,1};
        int threads[] = {1};
        for(int each:threads) {
            thread_nums = each < omp_get_max_threads() ? each : omp_get_max_threads();

            if (isBPSN)
                build_bwdpush();
            if (isFPSN and !isBPSN)
                build_dnpr();
            if (isRWIdx)
                build_rwidx_parallel();
//                build_rwidx();
        }
    } else{
        // load rwidx
        if (isRWIdx){
            rwpath = std::string("randwalks")+"_"+to_string(k);
            deserialize_idx();
        }
        if (isBPSN){
            rwpath = "output";
            deserialize_bwd();
        }
        if (!random_query){
            if (!isFPSN){
                prpath = std::string("/home/kester/pr_idx/") + "amazon.dnpr";
                deserialize_pr();
            }
            top_k_hub_cluster(sample);
        }
    }

    while (1) {
        int threads[] = {1};
        for(int each:threads){
            thread_nums = each<omp_get_max_threads()? each:omp_get_max_threads();
            double totaldnpprtime = 0;
            double totalembedtime = 0;
            for (int i = 0; i < sample; ++i) {
                timeElasped = 0;
                embedTimeElapsed = 0;
                /*
                vector<string> path;
                if (random_query)
                    generate_random_path(path,max_level);
                else{
                    path.emplace_back(hubcluster[i]);
                }*/
                init_container();
                vector<string> requested_path;
                requested_path.push_back(path_input);
                interactive_visualize(requested_path);
                cerr <<timeElasped<<endl;
                cout<<(timeElasped-embedTimeElapsed)<<endl;
                totaldnpprtime += (timeElasped-embedTimeElapsed);
                totalembedtime += embedTimeElapsed;
                if (!random_query){
                    int size = super2leaf[hubcluster[i]].size();
                    cout<<timeElasped/size<<endl;
                }
            }
            if (random_query)
                cout<<totaldnpprtime/sample<<" "<<totalembedtime/sample<<endl;
        }
        unsigned M = positions.rows();
        vector<vector<double>> coordinates(M);
        double * x = positions.data();
        double * y = positions.data()+M;


        std::string temp = path_input;
        std::string delimiter = "_";
        size_t pos = 0;
        std::string token;
        std::vector<std::string> nodes_name(coordinates.size());
        std::vector<std::string> parts(2);
        int index = 0;
        while ((pos = temp.find(delimiter)) != std::string::npos) {
            token = temp.substr(0, pos);
            parts[index] = token;
            index += 1;
            temp.erase(0, pos + delimiter.length());
        }
        char level_chr =  parts[1][1];
        int level = level_chr - '0';
        vector<int> super_nodes;
        if (level > 1) {
            super_nodes = super2super[path_input];
        } else {
            super_nodes = super2leaf[path_input];
        }

        bool pass = true;
        bool not_same = false;
        for (int i = 0; i < M; i++) {
            for (int j = i + 1; j < M; j++) {
                if (x[i] != x[j] || y[i] != y[j]) {
                    not_same = true;
                }
            }
        }
        for (int i = 0; i < M; i++) {
            if (isnan(x[i]) || isnan(y[i])) {
                pass = false;
            }


            coordinates[i] = vector<double>(4); // x y and radius
            coordinates[i][0] = x[i];
            coordinates[i][1] = y[i];
            coordinates[i][2] = radii[i];
            coordinates[i][3] = super_nodes[i];
            std::cerr << x[i];
            std::cerr << " ";
            std::cerr << y[i];
            std::cerr << " ";
            std::cerr << radii[i];
            std::cerr << "\n";
        }

        if (pass && not_same)
            return coordinates;
    }
}

vector<vector<double>> visualize(std::string path) {
     string path_input = path;
     int threads[] = {1};
     while (1) {
         int threads[] = {1};
         for(int each:threads){
             thread_nums = each<omp_get_max_threads()? each:omp_get_max_threads();
             double totaldnpprtime = 0;
             double totalembedtime = 0;

                 timeElasped = 0;
                 embedTimeElapsed = 0;
                 /*
                 vector<string> path;
                 if (random_query)
                     generate_random_path(path,max_level);
                 else{
                     path.emplace_back(hubcluster[i]);
                 }*/
                 init_container();
                 vector<string> requested_path;
                 requested_path.push_back(path_input);
                 interactive_visualize(requested_path);
                 cerr <<timeElasped<<endl;
                 cout<<(timeElasped-embedTimeElapsed)<<endl;
                 totaldnpprtime += (timeElasped-embedTimeElapsed);
                 totalembedtime += embedTimeElapsed;
         }
         unsigned M = positions.rows();
         vector<vector<double>> coordinates(M);
         double * x = positions.data();
         double * y = positions.data()+M;


         std::string temp = path_input;
         std::string delimiter = "_";
         size_t pos = 0;
         std::string token;
         std::vector<std::string> nodes_name(coordinates.size());
         std::vector<std::string> parts(2);
         int index = 0;
         while ((pos = temp.find(delimiter)) != std::string::npos) {
             token = temp.substr(0, pos);
             parts[index] = token;
             index += 1;
             temp.erase(0, pos + delimiter.length());
         }
         char level_chr =  parts[1][1];
         int level = level_chr - '0';
         vector<int> super_nodes;
         if (level > 1) {
             super_nodes = super2super[path_input];
         } else {
             super_nodes = super2leaf[path_input];
         }

         bool pass = true;
         bool not_same = false;
         for (int i = 0; i < M; i++) {
             for (int j = i + 1; j < M; j++) {
                 if (x[i] != x[j] || y[i] != y[j]) {
                     not_same = true;
                 }
             }
         }
         for (int i = 0; i < M; i++) {
             if (isnan(x[i]) || isnan(y[i])) {
                 pass = false;
             }


             coordinates[i] = vector<double>(4); // x y and radius
             coordinates[i][0] = x[i];
             coordinates[i][1] = y[i];
             coordinates[i][2] = radii[i];
             coordinates[i][3] = super_nodes[i];
             std::cerr << x[i];
             std::cerr << " ";
             std::cerr << y[i];
             std::cerr << " ";
             std::cerr << radii[i];
             std::cerr << "\n";
         }

         if (pass && not_same)
             return coordinates;
     }
}
