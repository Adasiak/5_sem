#include <stdio.h>  
#include <iostream>   
#include <string.h>       
#include <sstream.h>
#include <vector.h>

struct proc_data{
int t;
int i; //Numer procesu
int pi; //Priorytet
int ri; //Pozostaly czas wykonywania
int ti; //Czas wykonywania
bool isAvailable;
};

std::vector<proc_data> proc;

void ClearProc(proc_data *proc){
    proc->t=-1;
    proc->i=-1;
    proc->pi=-1;
    proc->ri=-1;
    proc->ti=-1;
    proc->isAvailable=true;
}

// void ReadData(){
//     std::string line;
//     std::stringstream linestream;
//     int temp_t;
//     proc_data temp_data;
//     int i=0;
    
    
//     while(!(std::cin.eof())){
//         std::getline(std::cin, line);
//         if(line!=""){
//             linestream.clear();
//             linestream.str(line);
//             linestream >> temp_t;
//             while(!linestream.eof()){
//                 temp_data.t=temp_t;
//                 temp_data.i =0;
//                 linestream >> temp_data.i;
//                 linestream >> temp_data.pi;
//                 linestream >> temp_data.ri;
//                 temp_data.ti = temp_data.ri;
//                 temp_data.isAvailable=true;
//                 if(temp_data.i!=0){
//                     proc.push_back(temp_data);
//                 }
//             }

//         }
//     }

// }
