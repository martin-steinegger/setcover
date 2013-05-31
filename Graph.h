//
//  Graph.h
//  graphcluster
//
//  Created by Martin Steinegger on 30.04.13.
//  Copyright (c) 2013 Martin Steinegger. All rights reserved.
//

#ifndef __graphcluster__Graph__
#define __graphcluster__Graph__

#include <iostream>
#include <algorithm>    // std::find

class Graph {

public:
    struct vertex_struct {
        unsigned short size;
        int  * element;
        bool * deleted;
    };
    
    Graph(int vertexCount) {
        this->vertexCount = vertexCount;
        this->vertex = new vertex_struct[vertexCount];
    }
    
    
    vertex_struct getNeighbor(int i) {
        vertex_struct retStruct = vertex[i];
        return retStruct;
    }
    
    void setVertexData(int i, int * vertex_data,int size){
        
        vertex[i].size = size;
        vertex[i].element  = new int [size];
        vertex[i].deleted  = new bool[size];
        vertex[i].element=vertex_data;
        std::fill(vertex[i].deleted, vertex[i].deleted+size, false);
        std::sort(vertex[i].element, vertex[i].element+size);
    }
    
    int removeNode(int i) {
        const vertex_struct vertex_to_remove = vertex[i];
        for(int i = 0; i < vertex_to_remove.size;i++){
            const vertex_struct search_for_vertex = vertex[vertex_to_remove.element[i]];
            int index_to_delete=search(search_for_vertex.element,search_for_vertex.size,i);
            if(index_to_delete != -1)
                search_for_vertex.deleted[index_to_delete]=true;
        }
        return vertex_to_remove.size;
    }
    
    ~Graph() {
        for (int i = 0; i < vertexCount; i++){
            delete[] this->vertex[i].element;
            delete[] this->vertex[i].deleted;
        }
        delete[] this->vertex;
    }
private:
    
    int vertexCount;
    vertex_struct * vertex;
    
    int search(const int * elements, int n, int elem)
    {
        unsigned int middle;
        unsigned int left = 0; // smallest index
        unsigned int right = n - 1; // highest Index
        
        
        while (true) // endless until found or not found
        {
            middle = left + ((right - left) / 2); // find middle
            
            if (right < left) //  everything was browesed (not found)
            {
                return -1;
            }
            
            if (elements[middle] == elem) // element found
            {
                return middle;
            }
            
            if (elements[middle] > elem)
                right = middle - 1; // search left 
            else
                left = middle + 1; // search right
        }
        return -1;
    }

};
#endif /* defined(__graphcluster__Graph__) */
