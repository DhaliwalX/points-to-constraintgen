#include<iostream>
#include<set>
#include<algorithm>
#include "Gpu.h"
#include "GpuFlow.h"

//constructor
GpuFlow :: GpuFlow(set<Gpu> & arg1)
{
     GpuFlowSet = arg1;
}

//Insertion
void GpuFlow :: insertGpu(Gpu & arg1)
{
     GpuFlowSet.insert(arg1);
}

//Deletion
void GpuFlow :: deleteGpu(Gpu & arg1)
{
    std :: set<Gpu> :: iterator it;
    it = GpuFlowSet.find(agr1);
    GpuFlowSet.erase(it,GpuFlowSet.end());
}

//union of two GpuFlow
void GpuFlow :: unionGpuFlow(GpuFlow & arg1)
{
     GpuFlowSet.insert(agr1.GpuFlowSet.begin(),agr1.GpuFlowSet.end());
}



