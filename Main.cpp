//
//  FixedSizeArrayStack.h
//  graphcluster
//
//  Created by Martin Steinegger on 21.05.13.
//  Copyright (c) 2013 Martin Steinegger. All rights reserved.
//

#include <iostream>
#include "LinearMultiArray.h"
#include "SetCover.h"
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <istream>
#include <cstring>
#include <stdlib.h> 
//
//#include "DBReader.h"
//
//void readInData(char * dbname,char * dbname_index){
//    DBReader* tdbr = new DBReader(dbname,dbname_index);
//    tdbr->open();
//    size_t targetDBSize = tdbr->getSize();
//    
//    
//    float max_weight=0;
//    int   max_element_count=0;
//    int   unique_element_id = 1;
//    int   unique_set_id = 1;
//    std::map<std::string, element_meta_data> element_meta_data_map;
//    std::vector<int>   element_id;
//    std::vector<float> element_weight;
//    int   set_element_count[targetDBSize+1];
//    for (int id = 0; id < targetDBSize; id++){
//        char* data =tdbr->getData(id);
//        std::istringstream stream(data);
//        std::string line;
//        float sum_weight=0;
//        int element_count = 0;
//        while (std::getline(stream, line)) {
//            std::istringstream in(line);
//            std::string name;
//            float weight;
//            in >> name >> weight;
//            sum_weight += weight;
//            element_meta_data * data = &element_meta_data_map[name];
//            if(data->element_id == 0){
//                data->element_id = unique_element_id++;
//                data->count++;
//            }
//            
//            element_id.push_back(data->element_id);
//            element_weight.push_back(weight);
//            
//            element_count++;
//        }
//        if(sum_weight > max_weight){
//            max_weight = sum_weight;
//        }
//        if(element_count > max_element_count){
//            max_element_count = element_count;
//        }
//        
//        set_element_count[unique_set_id++]=element_count;
//        
//        //      std::cout << sum_weight <<std::endl ;
//        
//    }
//    
////    std::cout <<  element_meta_data_map["Q2A5B4"].element_id << " " << element_meta_data_map["Q2A5B4"].count << std::endl;
//    std::cout <<  max_weight << std::endl;
//    
//    set_cover setcover(targetDBSize,element_id.size(),max_element_count);
//    for (int id = 0; id < targetDBSize; id++){
//        //TODO
////       setcover.add_set(3, 2,(const unsigned int*)&ids3,(const unsigned short*)&weight3, 2,(const unsigned int *)&element_count);
//    }
//    
//    tdbr->close();
//
//}
//  10 5
//  5 7 8 9 10
//  5 7
//  1 3 6 10
//  4 6 7 9 10
//  1 2 8 10
struct set_data {
    unsigned int ** sets;
    unsigned short ** weights;
    
    unsigned int * set_sizes;
    unsigned int * element_size_lookup;
    unsigned int set_count;
    unsigned int uniqu_element_count;
    unsigned int all_element_count;
    unsigned int max_weight;
};





set_data create_random_set_data(int set_size, int elment_count ){

// Open the stream

    int n,m;
    m=set_size;
    n=elment_count;
    set_data ret_struct;
    ret_struct.set_count = m;
    ret_struct.uniqu_element_count = n;
    
    
    unsigned int * element_buffer=(unsigned int *)malloc(n * sizeof(unsigned int));
    ret_struct.element_size_lookup = (unsigned int *)malloc((n+1) * sizeof(unsigned int));
    ret_struct.element_size_lookup[n]=0;
    unsigned int * element_size=ret_struct.element_size_lookup;
    memset(&element_size[0], 0, sizeof(unsigned int)*n);
    
    ret_struct.set_sizes = (unsigned int *)malloc((m+1) * sizeof(unsigned int));
    ret_struct.set_sizes[m]=0;
    unsigned int * set_sizes=ret_struct.set_sizes;
    
    ret_struct.sets = (unsigned int **)malloc(m * sizeof(unsigned int*));
    unsigned int ** sets   = ret_struct.sets;

    
    ret_struct.weights = (unsigned short **)malloc(m * sizeof(unsigned short*));
    unsigned short ** weights = ret_struct.weights;
    
    ret_struct.max_weight = 0;
    ret_struct.all_element_count=0;
    for(int i = 0; i<m; i++){
        int element_counter=0;
        for(int j = 0; j < 10; j++)        {
            int curr_element=rand()%(elment_count-2 + 1) + 1;
            if(curr_element == 0)
                continue;
            element_buffer[element_counter++]=curr_element;
            element_size[curr_element]++;
            ret_struct.all_element_count++;
        }
        if(ret_struct.max_weight < element_counter){
            ret_struct.max_weight = element_counter;
        }
        
        unsigned int * elements = new unsigned int[element_counter];
        memcpy(elements,element_buffer,sizeof(int)*element_counter);
        unsigned short * weight = new unsigned short[element_counter];
        std::fill_n(weight, element_counter, 1);
        
        weights[i] = weight;
        sets[i] = elements;
        set_sizes[i]=element_counter;
        
    }
    free(element_buffer);
    //    for(int i = 0; i<m; i++){
    //        for(int j = 0; j < set_size[i]; j ++)
    //            std::cout << sets[i][j] << ":" << weights[i][j] << " ";
    //
    //
    //        std::cout << std::endl;
    //    }
    
    return ret_struct;
}



bool validate_result(std::list<set *> * ret,unsigned int uniqu_element_count){
    std::list<set *>::const_iterator iterator;
    unsigned int result_element_count=0;
    for (iterator = ret->begin(); iterator != ret->end(); ++iterator) {
        set::element * element =(*iterator)->elements;
        do{
            result_element_count++;
        }while((element=element->next)!=NULL);
    }
    return (uniqu_element_count==result_element_count)? true : false;
}


set_data read_in_set_data(char * dbname){
    // Open the stream
    std::ifstream file(dbname);
    
    if ( !file )
    {
        std::cout << "File dosen't exist" << std::endl;
    }
    std::stringstream buffer;
        
    buffer << file.rdbuf();
    
    file.close();

    set_data ret_struct;
    
    std::string line;
    std::getline(buffer, line);
    std::istringstream line_stream(line);
    int n,m;
    line_stream >> n >> m;
    ret_struct.set_count = m;
    ret_struct.uniqu_element_count = n;   
    
    
    unsigned int * element_buffer=(unsigned int *)malloc(n * sizeof(unsigned int));
    unsigned int * element_size=new unsigned int[n+2];
    memset(&element_size[0], 0, sizeof(unsigned int)*(n+2));
    ret_struct.element_size_lookup = element_size;
    unsigned int * set_size=new unsigned int[m];
    
    ret_struct.set_sizes = set_size;
    unsigned int ** sets   = new unsigned int*[m];
    ret_struct.sets = sets;
    unsigned short ** weights = new unsigned short*[m];
    ret_struct.weights = weights;
    ret_struct.max_weight = 0;
    ret_struct.all_element_count=0;

    for(int i = 0; i<m; i++){
        std::getline(buffer, line);
        std::string token;
        int element_counter=0;
        line_stream.clear();
        line_stream.str(line);
        while (std::getline(line_stream, token, ' '))
        {
            int curr_element=atoi(token.c_str());
            if(curr_element == 0)
                continue;
            element_buffer[element_counter++]=curr_element;
            element_size[curr_element]++;
            ret_struct.all_element_count++;
        }
        if(ret_struct.max_weight < element_counter){
            ret_struct.max_weight = element_counter;
        }

        unsigned int * elements = new unsigned int[element_counter];
        memcpy(elements,element_buffer,sizeof(int)*element_counter);
        unsigned short * weight = new unsigned short[element_counter];
        std::fill_n(weight, element_counter, 1);

        weights[i] = weight;
        sets[i] = elements;
        set_size[i]=element_counter;

    }
    
//    for(int i = 0; i<m; i++){
//        for(int j = 0; j < set_size[i]; j ++)
//            std::cout << sets[i][j] << ":" << weights[i][j] << " ";
//
//        
//        std::cout << std::endl;
//    }

    return ret_struct;
}



int main()
{

    std::cout << "start readin";
    std::cout.flush();
    set_data set_data = read_in_set_data("/Users/aluucard/Documents/workspace/graphcluster/graphcluster/webdocs.dat");
//    set_data set_data = create_random_set_data(1000000,1000000);
    std::cout << " --- Done" << std::endl;
    std::cout << "init setcover";
    std::cout.flush();


    
    set_cover setcover(set_data.set_count,
                       set_data.uniqu_element_count,
                       set_data.max_weight,
                       set_data.all_element_count,
                       set_data.element_size_lookup
                       );

    for(int i = 0; i < set_data.set_count; i++){
        setcover.add_set(i+1, set_data.set_sizes[i]
                         ,(const unsigned int*)set_data.sets[i],
                          (const unsigned short*)set_data.weights[i],
                          set_data.set_sizes[i]);
        free(set_data.sets[i]);
        free(set_data.weights[i]);

    }
    std::cout << " --- Done" << std::endl;

//    readInData("/Users/aluucard/Documents/workspace/graphcluster/graphcluster/test_data/graphdb_final",
//               "/Users/aluucard/Documents/workspace/graphcluster/graphcluster/test_data/graphdb_final.index");

    std::cout << "execute setcover" ;
    std::cout.flush();

    std::list<set *> ret= setcover.execute_set_cover();
    std::cout << " --- Done" << std::endl;
    std::cout << "validate result ";
    if(validate_result(&ret,set_data.uniqu_element_count))
        std::cout << " VALID ";
    else
        std::cout << " NOT VALID ";

    std::cout << " --- Done" << std::endl;

    
    std::cout << "write result setcover" << std::endl;
    std::cout.flush();

    std::list<set *>::const_iterator iterator;
    for (iterator = ret.begin(); iterator != ret.end(); ++iterator) {
        std::cout << "set id " << (*iterator)->set_id << " Element: ";
        set::element * element =(*iterator)->elements;
        do{
            std::cout << element->element_id << ", ";
        }while((element=element->next)!=NULL);
        std::cout << std::endl;
    }

}

