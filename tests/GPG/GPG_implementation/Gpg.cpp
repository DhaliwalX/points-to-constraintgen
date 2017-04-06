#include<iostream>
#include<vector>
#include<algorithm>
#include "Gpu.h"
#include "GpuFlow.h"
#include "Gpb.h"
#include "Gpg.h"

//constructor
Gpg :: Gpg(Gpb * arg1, Gpbset<Gpb*> & arg2, Gpb *exit)
 : entryGpb_{ arg1 }, gpbSet_(arg2.begin(), arg2.end()), exitGpb_{ exit }
{ }

