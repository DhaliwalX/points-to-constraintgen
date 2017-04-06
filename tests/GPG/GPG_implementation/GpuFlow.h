#ifndef GPUFLOW_H
#define GPUFLOW_H

//header file for GPUFLOW
class GpuFlow
{
    private :
        //set containing GPUs(flow from one GPB to another) 
        std::set<Gpu> GpuFlowSet;
   
    public :
       //constructor
       GpuFlow(set<Gpu> &);
        
       //Insertion
       void insertGpu(Gpu &);
 
       //Deletion
       void deleteGpu(Gpu &);

       //Union:: union will be "this" set 
       void unionGpuFlow(GpuFlow &);
};

#endif
