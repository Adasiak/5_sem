#include <iostream>     
#include <string>       
#include <sstream>
#include <vector>

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

void ReadData(){
    std::string line;
    std::stringstream linestream;
    int temp_t;
    proc_data temp_data;
    int i=0;
    
    
    while(!(std::cin.eof())){
        std::getline(std::cin, line);
        if(line!=""){
            linestream.clear();
            linestream.str(line);
            linestream >> temp_t;
            while(!linestream.eof()){
                temp_data.t=temp_t;
                temp_data.i =0;
                linestream >> temp_data.i;
                linestream >> temp_data.pi;
                linestream >> temp_data.ri;
                temp_data.ti = temp_data.ri;
                temp_data.isAvailable=true;
                if(temp_data.i!=0){
                    proc.push_back(temp_data);
                }
            }

        }
    }

}

int main(int argc, char *argv[]){

if(argc!=3){
    std::cout << "Złe argumenty \n";
    return 0;
}

int Num_of_processors = std::stoi(argv[2]);
int Id=std::stoi(argv[1]);
proc_data processors[Num_of_processors];
for (int i=0; i<Num_of_processors; i++){
    ClearProc(&(processors[i]));
}

int t=0;
int j,k;
int min, minId;
int procId;
ReadData();

int Num_of_proc = proc.size();

switch (Id){
    case 0:
    //FCFS
    std::cout << "FCFS \n";
    while(Num_of_proc>0){
        //Sprawdzenie czy procesor jest wolny
        for(j=0; j<Num_of_processors; j++){
            if(processors[j].isAvailable==true){
                //Wybór procesu
                for(k=0;k<proc.size();k++){
                    //Wybor pierwszego wolnego procesu (Procesy sa dodane do wektora w kolejnosci przychodzenia)
                    if((proc[k].isAvailable==true) && (proc[k].ri>0) && ((proc[k].t)<=t)){
                        //Zajecie procesora
                        proc[k].isAvailable=false;
                        processors[j]=proc[k];
                        k=proc.size();
                    }
                }
            }
            
        }

        //Przeniesienie na wolne procesory
        for(j=0; j<Num_of_processors; j++){  
            if(processors[j].isAvailable==true){
                for(k=j+1; k<Num_of_processors; k++){
                    //Znalezienie pierwszego mozliwego procesu
                    if(processors[k].isAvailable==false){
                        processors[j]=processors[k];
                        ClearProc(&(processors[k]));
                        k=Num_of_processors;
                    }

                }

            }
        }


        //Wyswietlenie aktualnego stanu procesorow
        std::cout << t;
        for(j=0; j<Num_of_processors; j++){
            std::cout << " " << processors[j].i;
        }
        std::cout << "\n";

        //Wykonanie kwantu czasu
        ++t;
        for(j=0; j<Num_of_processors; j++){
            --(processors[j].ri);
            //Sprawdzenie czy proces zostal wykonany
            if(processors[j].ri==0){
                //Proces zostal wykonany
                ClearProc(&(processors[j]));
                --Num_of_proc;
            }
        }



    }
    break;
    case 1:
    //SJF
    std::cout << "SJF \n";
    while(Num_of_proc>0){
        //Sprawdzenie czy procesor jest wolny
        for(j=0; j<Num_of_processors; j++){
            if(processors[j].isAvailable==true){
                //Znalezienie najkrotszego procesu
                min=100000000;
                minId=-1;
                for(k=0; k<proc.size();k++){
                    if((proc[k].isAvailable==true) && (proc[k].ri>0) && ((proc[k].t)<=t) && (proc[k].ti)<min){
                        min=proc[k].ti;
                        minId=proc[k].i-1;
                    }
                }
                //Zajecie procesora znalezionym procesem
                if(minId>=0){
                proc[minId].isAvailable=false;
                processors[j]=proc[minId];
                }
                
            }
            
        }

        //Przeniesienie na wolne procesory
        for(j=0; j<Num_of_processors; j++){  
            if(processors[j].isAvailable==true){
                for(k=j+1; k<Num_of_processors; k++){
                    //Znalezienie pierwszego mozliwego procesu
                    if(processors[k].isAvailable==false){
                        processors[j]=processors[k];
                        ClearProc(&(processors[k]));
                        k=Num_of_processors;
                    }

                }

            }
        }

        //Wyswietlenie aktualnego stanu procesorow
        std::cout << t;
        for(j=0; j<Num_of_processors; j++){
            std::cout << " " << processors[j].i;
        }
        std::cout << "\n";

        //Wykonanie kwantu czasu
        ++t;
        for(j=0; j<Num_of_processors; j++){
            --(processors[j].ri);
            //Sprawdzenie czy proces zostal wykonany
            if(processors[j].ri==0){
                //Proces zostal wykonany
                ClearProc(&(processors[j]));
                --Num_of_proc;
            }
        }



    }
    break;
    case 2:
    //SRTF
    std::cout << "SRTF \n";
    while(Num_of_proc>0){
        //Sprawdzenie czy procesor jest wolny
        for(j=0; j<Num_of_processors; j++){
            if(processors[j].isAvailable==true){
                //Znalezienie najkrotszego procesu
                min=100000000;
                minId=-1;
                for(k=0; k<proc.size();k++){
                    if((proc[k].isAvailable==true) && (proc[k].ri>0) && ((proc[k].t)<=t) && (proc[k].ri)<min){
                        min=proc[k].ri;
                        minId=proc[k].i-1;
                    }
                }
                //Zajecie procesora znalezionym procesem
                if(minId>=0){
                proc[minId].isAvailable=false;
                processors[j]=proc[minId];
                }
                
            }
            
        }
        //Wyswietlenie aktualnego stanu procesorow
        std::cout << t;
        for(j=0; j<Num_of_processors; j++){
            std::cout << " " << processors[j].i;
        }
        std::cout << "\n";

        //Wykonanie kwantu czasu
        ++t;
        for(j=0; j<Num_of_processors; j++){
            --(processors[j].ri);
            //Sprawdzenie czy proces zostal wykonany
            if(processors[j].ri==0){
                //Proces zostal wykonany
                --Num_of_proc;
            }
            //Wywlaszczanie
            procId=(processors[j].i)-1;
            processors[j].isAvailable=true;
            proc[procId]=processors[j];
            ClearProc(&(processors[j]));
        }



    }
    break;

}

return 0;
}