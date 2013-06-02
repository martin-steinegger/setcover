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
    int n,m;
    m=set_size;
    n=elment_count;
    set_data ret_struct;
    ret_struct.set_count = m;
    ret_struct.uniqu_element_count = n;
    
    
    unsigned int * element_buffer=(unsigned int *)malloc(n * sizeof(unsigned int));
    ret_struct.element_size_lookup = (unsigned int *)malloc((n+2) * sizeof(unsigned int));
    unsigned int * element_size=ret_struct.element_size_lookup;
    memset(&element_size[0], 0, sizeof(unsigned int)*n+2);
    
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

    return ret_struct;
}



int main()
{

    std::cout << "start readin";
    std::cout.flush();
    set_data set_data = read_in_set_data("testdatafile");
//    set_data set_data = create_random_set_data(29000000,29000000);
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

