#ifndef GPG_H
#define GPG_H

//header file for GPG
class Gpg
{
    private:
        //Entry GPB of the GPG
        Gpb * EntryGpb;
        Gpb * ExitGpb;
        //vector of GPBs which are contaioned in a GPG
        std :: vector<Gpb*> GpbSet;

    public:
        //constructor
        Gpg(Gpb *, Gpb *, vector<Gpb*> &);

        // get the entry GPB

};

#endif
